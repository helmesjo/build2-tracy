# libtracy-client - Tracy Profiler

This is a `build2` package for the [`tracy`](https://github.com/wolfpld/tracy)
C++ library. It provides the client part of Tracy, a real time, nanosecond
resolution, remote telemetry, hybrid frame and sampling profiler for games
and other applications: the instrumentation macros and the profiler that
collects the data and sends it to a Tracy server, such as `tracy-capture`
from the `tracy-tools` package or the Tracy profiler application.

Unlike upstream, this package uses the `liblz4`, `librpmalloc`,
`libspscqueue`, `concurrentqueue`, and `libbacktrace` packages instead of
Tracy's private copies of these libraries. As a result, symbols of other
processes' images cannot be resolved (external process symbolization,
`TRACY_DEBUGINFOD`, and `TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT` are not
supported) and `librpmalloc` must be configured with
`config.librpmalloc.override=false`.


## Usage

To start using `libtracy-client` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libtracy-client ^0.14.1
```

Then import the library in your `buildfile`:

```
import libs = libtracy-client%lib{TracyClient}
```


## Importable targets

This package provides the following importable targets:

```
lib{TracyClient}
```

The Tracy client library. Its headers are included as `<tracy/Tracy.hpp>`,
`<tracy/TracyC.h>`, and the other `<tracy/Tracy*.hpp>` integration headers
(`<tracy/TracyOpenGL.hpp>`, `<tracy/TracyVulkan.hpp>`, etc).


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libtracy_client.enable    ?= true
[bool] config.libtracy_client.on_demand ?= false
```

The `enable` variable enables the profiler (`TRACY_ENABLE`). If it is
`false`, the instrumentation macros expand to nothing.

The `on_demand` variable makes the client only collect data while a server
is connected (`TRACY_ON_DEMAND`).

Both are exported to consumers, since they change what the public headers
expand to. The other upstream `TRACY_*` options are not exposed.
