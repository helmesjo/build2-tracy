// Compatibility shim: Tracy sources include "backtrace.hpp" from a modified
// C++ copy of libbacktrace. libbacktrace exports <backtrace.h> with the same
// API in the global namespace, except for backtrace_create_state_for_file()
// and the backtrace_pcinfo() callback, which in the copy also receives the
// start address of the function containing pc.
//
#pragma once

#include <cstdint>

#include <backtrace.h>

namespace tracy
{
  typedef int (*backtrace_full_callback_lowaddr) (void* data,
                                                  std::uintptr_t pc,
                                                  std::uintptr_t lowaddr,
                                                  const char* filename,
                                                  int lineno,
                                                  const char* function);

  using ::backtrace_pcinfo;

  // Obtain the function start address from the symbol table (the enclosing
  // symbol's value, 0 if unknown) and pass it along with each frame.
  //
  inline int
  backtrace_pcinfo (backtrace_state* state,
                    std::uintptr_t pc,
                    backtrace_full_callback_lowaddr callback,
                    backtrace_error_callback error_callback,
                    void* data)
  {
    struct context
    {
      backtrace_full_callback_lowaddr callback;
      void* data;
      std::uintptr_t lowaddr;
    } c {callback, data, 0};

    ::backtrace_syminfo (
      state,
      pc,
      [] (void* d, std::uintptr_t, const char*, std::uintptr_t symval, std::uintptr_t)
      {
        static_cast<context*> (d)->lowaddr = symval;
      },
      [] (void*, const char*, int) {},
      &c);

    return ::backtrace_pcinfo (
      state,
      pc,
      [] (void* d, std::uintptr_t pc, const char* f, int l, const char* fn) -> int
      {
        auto* x (static_cast<context*> (d));
        return x->callback (x->data, pc, x->lowaddr, f, l, fn);
      },
      error_callback,
      &c);
  }

  // Tracy uses this to symbolize images of external processes. Unmodified
  // libbacktrace has no equivalent, so report that no state is available,
  // which Tracy treats as unresolvable.
  //
  inline backtrace_state*
  backtrace_create_state_for_file (const char*,
                                   int,
                                   backtrace_error_callback,
                                   void*)
  {
    return nullptr;
  }
}
