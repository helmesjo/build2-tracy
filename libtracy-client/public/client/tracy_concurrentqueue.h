// Compatibility shim: Tracy sources include "tracy_concurrentqueue.h" and use
// tracy::moodycamel::ConcurrentQueue, a modified copy of moodycamel's queue.
// This adapts the unmodified queue from the concurrentqueue package
// (<concurrentqueue/concurrentqueue.h>) to the parts of that interface Tracy
// uses:
//
// - ExplicitProducer::enqueue_begin()/get_tail_index() (used by the inline
//   TracyLfqPrepare/TracyLfqCommit macros): enqueue_begin() hands out a
//   per-producer staging item and the tail index "store" publishes it with
//   a regular enqueue.
//
// - ProducerToken with the producing thread's id, and
//   try_dequeue_bulk_single(), which drains one producer at a time and
//   reports its thread id before passing on the items.
//
// - readopt_orphaned_producer(): like the original, the producer list works
//   on zero-initialized queue memory, so a producer created before the
//   queue's constructor runs can be linked back in afterwards.
//
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

#include <concurrentqueue/concurrentqueue.h>

#include "../common/TracyAlloc.hpp"
#include "../common/TracyForceInline.hpp"
#include "../common/TracySystem.hpp"

namespace tracy
{
  // Defined by the original header and used elsewhere in Tracy (avoids
  // MSVC's "conditional expression is constant" warning).
  //
  namespace
  {
    template <bool>
    struct compile_time_condition
    {
      static const bool value = false;
    };

    template <>
    struct compile_time_condition<true>
    {
      static const bool value = true;
    };
  }

namespace moodycamel
{
  struct ConcurrentQueueDefaultTraits: ::moodycamel::ConcurrentQueueDefaultTraits
  {
    static const std::size_t BLOCK_SIZE = 64 * 1024;

    static inline void* (malloc) (std::size_t n) {return tracy_malloc (n);}
    static inline void (free) (void* p) {tracy_free (p);}
  };

  template <typename T, typename Traits = ConcurrentQueueDefaultTraits>
  class ConcurrentQueue;

  struct ProducerToken;

  namespace details
  {
    // Must be usable when zero-initialized (see readopt_orphaned_producer()).
    //
    struct ProducerBase
    {
      ProducerBase* next;
      std::atomic<bool> inactive;
      ProducerToken* token;
      std::uint32_t threadId;

      // Number of items enqueued (written by the producer) and dequeued
      // (written by the consumer). A producer is only recycled for another
      // thread once it is drained, so that its remaining items are not
      // attributed to the new thread.
      //
      std::atomic<std::uint64_t> enqueued;
      std::atomic<std::uint64_t> dequeued;

      bool
      empty () const
      {
        return enqueued.load (std::memory_order_acquire) ==
               dequeued.load (std::memory_order_acquire);
      }
    };
  }

  struct ProducerToken
  {
    template <typename T, typename Traits>
    explicit
    ProducerToken (ConcurrentQueue<T, Traits>& q)
        : producer (q.recycle_or_create_producer ())
    {
      producer->token = this;
      producer->threadId = detail::GetThreadHandleImpl ();
    }

    ProducerToken (ProducerToken&& x) noexcept
        : producer (x.producer)
    {
      x.producer = nullptr;

      if (producer != nullptr)
        producer->token = this;
    }

    ProducerToken&
    operator= (ProducerToken&& x) noexcept
    {
      if (this != &x)
      {
        release ();

        producer = x.producer;
        x.producer = nullptr;

        if (producer != nullptr)
          producer->token = this;
      }

      return *this;
    }

    ~ProducerToken () {release ();}

    ProducerToken (const ProducerToken&) = delete;
    ProducerToken& operator= (const ProducerToken&) = delete;

    details::ProducerBase* producer;

  private:
    void
    release () noexcept
    {
      if (producer != nullptr)
      {
        producer->token = nullptr;
        producer->inactive.store (true, std::memory_order_release);
        producer = nullptr;
      }
    }
  };

  struct ConsumerToken
  {
    template <typename T, typename Traits>
    explicit
    ConsumerToken (ConcurrentQueue<T, Traits>&)
        : current (nullptr), buffer (nullptr) {}

    ~ConsumerToken () {tracy_free (buffer);}

    ConsumerToken (const ConsumerToken&) = delete;
    ConsumerToken& operator= (const ConsumerToken&) = delete;

    details::ProducerBase* current;
    void* buffer;
  };

  template <typename T, typename Traits>
  class ConcurrentQueue
  {
  public:
    typedef typename Traits::index_t index_t;
    typedef typename Traits::size_t size_t;

  private:
    typedef ::moodycamel::ConcurrentQueue<T, Traits> queue_type;

  public:
    struct ExplicitProducer: details::ProducerBase
    {
      // Publishes the staged item when the TracyLfqCommit macro stores the
      // new tail index.
      //
      struct TailIndex
      {
        ExplicitProducer* producer;

        tracy_force_inline void
        store (index_t, std::memory_order)
        {
          ExplicitProducer& p (*producer);

          if (p.queue->enqueue (p.upstream, std::move (p.item)))
            p.enqueued.store (p.enqueued.load (std::memory_order_relaxed) + 1,
                              std::memory_order_release);
        }
      };

      explicit
      ExplicitProducer (queue_type& q)
          : details::ProducerBase {nullptr, {false}, nullptr, 0, {0}, {0}},
            queue (&q), upstream (q), item (), tail {this} {}

      tracy_force_inline T*
      enqueue_begin (index_t& currentTailIndex)
      {
        currentTailIndex = 0;
        return &item;
      }

      tracy_force_inline TailIndex&
      get_tail_index ()
      {
        return tail;
      }

      queue_type* queue;
      ::moodycamel::ProducerToken upstream;
      T item;
      TailIndex tail;
    };

    explicit
    ConcurrentQueue (size_t capacity = 6 * Traits::BLOCK_SIZE)
        : producers_ (nullptr), queue_ (capacity) {}

    ~ConcurrentQueue ()
    {
      for (auto* p (producers_.load (std::memory_order_relaxed)); p != nullptr; )
      {
        auto* n (p->next);

        if (p->token != nullptr)
          p->token->producer = nullptr;

        auto* ep (static_cast<ExplicitProducer*> (p));
        ep->~ExplicitProducer ();
        (Traits::free) (ep);

        p = n;
      }
    }

    ConcurrentQueue (const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator= (const ConcurrentQueue&) = delete;

    ExplicitProducer*
    get_explicit_producer (const ProducerToken& token)
    {
      return static_cast<ExplicitProducer*> (token.producer);
    }

    // If a producer token is created before the constructor of a statically
    // allocated queue runs, the constructor orphans it by resetting the
    // producer list. Link such a producer back into the list.
    //
    bool
    readopt_orphaned_producer (ExplicitProducer* producer)
    {
      for (auto* p (producers_.load (std::memory_order_acquire));
           p != nullptr;
           p = p->next)
      {
        if (p == producer)
          return false;
      }

      add_producer (producer);
      return true;
    }

    // Dequeue up to 8192 items from a single producer, calling
    // notifyThread(threadId) and then processData(T*, size_t) for them.
    // Producers are visited round-robin across calls.
    //
    template <typename NotifyThread, typename ProcessData>
    size_t
    try_dequeue_bulk_single (ConsumerToken& token,
                             NotifyThread notifyThread,
                             ProcessData processData)
    {
      const size_t max (8192);

      if (token.buffer == nullptr)
      {
        token.buffer = tracy_malloc (sizeof (T) * max);

        if (token.buffer == nullptr)
          return 0;
      }

      auto* head (producers_.load (std::memory_order_acquire));

      if (head == nullptr)
        return 0;

      T* items (static_cast<T*> (token.buffer));

      auto* start (token.current != nullptr ? token.current : head);
      auto* p (start);

      do
      {
        auto* ep (static_cast<ExplicitProducer*> (p));
        auto* next (p->next != nullptr ? p->next : head);

        size_t n (queue_.try_dequeue_bulk_from_producer (ep->upstream, items, max));

        if (n != 0)
        {
          // Read the thread id before marking the items dequeued, after
          // which the producer may be recycled.
          //
          std::uint32_t tid (p->threadId);
          p->dequeued.fetch_add (n, std::memory_order_release);

          notifyThread (tid);
          processData (items, n);

          token.current = next;
          return n;
        }

        p = next;
      }
      while (p != start);

      return 0;
    }

  private:
    friend struct ProducerToken;

    details::ProducerBase*
    recycle_or_create_producer ()
    {
      for (auto* p (producers_.load (std::memory_order_acquire));
           p != nullptr;
           p = p->next)
      {
        bool expected (true);

        if (p->inactive.load (std::memory_order_relaxed) &&
            p->empty () &&
            p->inactive.compare_exchange_strong (expected,
                                                 false,
                                                 std::memory_order_acquire,
                                                 std::memory_order_relaxed))
          return p;
      }

      void* m ((Traits::malloc) (sizeof (ExplicitProducer)));
      auto* p (new (m) ExplicitProducer (queue_));

      add_producer (p);
      return p;
    }

    void
    add_producer (details::ProducerBase* p)
    {
      auto* head (producers_.load (std::memory_order_relaxed));

      do
      {
        p->next = head;
      }
      while (!producers_.compare_exchange_weak (head,
                                                p,
                                                std::memory_order_release,
                                                std::memory_order_relaxed));
    }

    std::atomic<details::ProducerBase*> producers_;
    queue_type queue_;
  };
}
}
