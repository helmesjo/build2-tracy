// Compatibility shim: Tracy sources include "tracy_lz4.hpp" and use the
// LZ4 API from namespace tracy (a namespaced copy of LZ4). liblz4 exports
// <lz4.h> with the same API in the global namespace.
//
#pragma once

#include <lz4.h>

namespace tracy
{
  using ::LZ4_stream_t;
  using ::LZ4_streamDecode_t;

  using ::LZ4_createStream;
  using ::LZ4_freeStream;
  using ::LZ4_resetStream;
  using ::LZ4_compress_fast_continue;

  using ::LZ4_createStreamDecode;
  using ::LZ4_freeStreamDecode;
  using ::LZ4_setStreamDecode;
  using ::LZ4_decompress_safe;
  using ::LZ4_decompress_safe_continue;
}
