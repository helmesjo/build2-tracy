// Compatibility shim: Tracy sources include "tracy_SPSCQueue.h" and use
// tracy::SPSCQueue<T>, a copy of rigtorp::SPSCQueue that allocates with
// tracy_malloc()/tracy_free(). libspscqueue exports <rigtorp/SPSCQueue.h>.
//
#pragma once

#include <cstddef>

#include <rigtorp/SPSCQueue.h>

#include "../common/TracyAlloc.hpp"

namespace tracy
{
  template <typename T>
  struct SPSCQueueAllocator
  {
    using value_type = T;

    SPSCQueueAllocator () noexcept = default;

    template <typename U>
    SPSCQueueAllocator (const SPSCQueueAllocator<U>&) noexcept {}

    T*
    allocate (std::size_t n)
    {
      return static_cast<T*> (tracy_malloc (sizeof (T) * n));
    }

    void
    deallocate (T* p, std::size_t) noexcept
    {
      tracy_free (p);
    }

    template <typename U>
    bool operator== (const SPSCQueueAllocator<U>&) const noexcept {return true;}

    template <typename U>
    bool operator!= (const SPSCQueueAllocator<U>&) const noexcept {return false;}
  };

  template <typename T>
  using SPSCQueue = rigtorp::SPSCQueue<T, SPSCQueueAllocator<T>>;
}
