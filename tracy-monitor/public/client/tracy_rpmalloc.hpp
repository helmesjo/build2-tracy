// Compatibility shim: Tracy sources include "tracy_rpmalloc.hpp" and use the
// rpmalloc 1.4 API from namespace tracy (a namespaced copy of rpmalloc).
// librpmalloc exports <rpmalloc.h> with the rpmalloc 2 API in the global
// namespace, where initialization and thread finalization take different
// arguments.
//
#pragma once

#include <rpmalloc.h>

namespace tracy
{
  using ::rpmalloc;
  using ::rpfree;
  using ::rprealloc;

  using ::rpmalloc_finalize;
  using ::rpmalloc_thread_initialize;

  inline int
  rpmalloc_initialize ()
  {
    return ::rpmalloc_initialize (nullptr);
  }

  inline void
  rpmalloc_thread_finalize (int /* release_caches */)
  {
    ::rpmalloc_thread_finalize ();
  }
}
