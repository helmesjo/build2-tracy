// Compatibility shim: Tracy sources define XXH_INLINE_ALL and include
// "tracy_xxhash.h" (a copy of xxHash). xxhash exports <xxhash.h>, which
// supports the same inline mode.
//
#include <xxhash.h>
