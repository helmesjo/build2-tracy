// Compatibility shim: Tracy sources include "tracy_robin_hood.h" and use the
// robin_hood containers from namespace tracy (a copy of robin-hood-hashing
// with its namespace renamed). robin-hood-hashing exports <robin_hood.h>
// with the same containers in namespace robin_hood.
//
#pragma once

#include <robin_hood.h>

namespace tracy
{
  using ::robin_hood::unordered_flat_map;
  using ::robin_hood::unordered_flat_set;
  using ::robin_hood::unordered_node_map;
  using ::robin_hood::unordered_node_set;
  using ::robin_hood::unordered_map;
  using ::robin_hood::unordered_set;
}
