// Compatibility shim: Tracy sources include "tracy_pdqsort.h" and call
// pdqsort() and pdqsort_branchless() from namespace tracy (a namespaced copy
// of pdqsort). libpdqsort exports <libpdqsort/pdqsort.h> with the same
// functions in the global namespace.
//
#pragma once

#include <libpdqsort/pdqsort.h>

namespace tracy
{
  using ::pdqsort;
  using ::pdqsort_branchless;
}
