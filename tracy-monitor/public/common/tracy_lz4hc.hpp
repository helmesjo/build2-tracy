// Compatibility shim: Tracy sources include "tracy_lz4hc.hpp" and use the
// LZ4 HC API from namespace tracy (a namespaced copy of LZ4). liblz4
// exports <lz4hc.h> with the same API in the global namespace.
//
#pragma once

#include "tracy_lz4.hpp"

#include <lz4hc.h>

namespace tracy
{
  using ::LZ4_streamHC_t;

  using ::LZ4_createStreamHC;
  using ::LZ4_freeStreamHC;
  using ::LZ4_resetStreamHC;
  using ::LZ4_compress_HC_continue;
}
