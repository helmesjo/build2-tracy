# Missing third-party packages

Inventory of third-party dependencies that upstream Tracy bundles, and how
the packages in this repository consume them. Already unbundled packages
are listed for context.

**Already external** (`depends` of `libtracy-client`):

| Package | Source | Replaces |
|---|---|---|
| `libspscqueue` | dir `../tracy-deps/build2-SPSCQueue` (`^1.1.0-`) | `client/tracy_SPSCQueue.h` |
| `liblz4` | cppget testing (`^1.10.0`) | `common/tracy_lz4{,hc}.{hpp,cpp}` |
| `librpmalloc` | dir `../tracy-deps/build2-rpmalloc` (`^2.0.1-`, `override=false`) | `client/tracy_rpmalloc.{hpp,cpp}` |
| `concurrentqueue` | cppget stable (`^1.0.3`) | `client/tracy_concurrentqueue.h` |
| `libbacktrace` | dir `../tracy-deps/build2-libbacktrace` (`^0.0.1-`, not on Windows) | `libbacktrace/` |

---

## Currently vendored in this repository

None. `libtracy-client` does not compile, link, or ship any of Tracy's
bundled copies of the libraries above: those upstream files are not
symlinked into the package at all.

Unmodified upstream Tracy sources still `#include` the bundled copies by
their upstream paths (for example `"tracy_SPSCQueue.h"` or
`"../libbacktrace/backtrace.hpp"`). To keep those sources unpatched, the
package has small compatibility headers of its own at those paths. They
contain no code from the bundled copies. Each one includes the packaged
library's header and maps the names and signatures Tracy expects onto it:

| Compatibility header | Path | Forwards to |
|---|---|---|
| SPSCQueue | `libtracy-client/public/client/tracy_SPSCQueue.h` | `<rigtorp/SPSCQueue.h>`: `tracy::SPSCQueue<T>` as `rigtorp::SPSCQueue<T>` with a `tracy_malloc()` allocator |
| lz4 | `libtracy-client/public/common/tracy_lz4{,hc}.hpp` | `<lz4.h>`, `<lz4hc.h>`: the `LZ4_*` functions and types Tracy uses, made visible in `namespace tracy` |
| rpmalloc | `libtracy-client/public/client/tracy_rpmalloc.hpp` | `<rpmalloc.h>`: rpmalloc 1.4 style `rpmalloc_initialize()` and `rpmalloc_thread_finalize(int)` on top of the rpmalloc 2 API |
| concurrentqueue | `libtracy-client/public/client/tracy_concurrentqueue.h` | `<concurrentqueue/concurrentqueue.h>`: Tracy's producer reserve/commit and per-thread bulk dequeue built on the moodycamel public API |
| libbacktrace | `libtracy-client/public/libbacktrace/backtrace.hpp` | `<backtrace.h>`: `backtrace_pcinfo()` with the function start address Tracy's callbacks expect. `backtrace_create_state_for_file()` (a Tracy-only addition) is not supported. |

Upstream `TracyClient.cpp` and `TracyCallstack.cpp` also `#include` the
bundled copies' `.cpp` files (`common/tracy_lz4.cpp`,
`client/tracy_rpmalloc.cpp`, `libbacktrace/*.cpp`). Those paths hold empty
placeholder files, so nothing from the bundled copies is compiled.
