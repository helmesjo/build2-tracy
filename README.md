# tracy - Frame and sampling profiler

This is a `build2` package repository for [`tracy`](https://github.com/wolfpld/tracy),
a real time, nanosecond resolution, remote telemetry, hybrid frame and
sampling profiler for games and other applications.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`tracy` in your `build2`-based project, then instead see the accompanying
`PACKAGE-README.md` files:

- [`libtracy-client/PACKAGE-README.md`](libtracy-client/PACKAGE-README.md)
- [`tracy-tools/PACKAGE-README.md`](tracy-tools/PACKAGE-README.md)
- [`tracy-monitor/PACKAGE-README.md`](tracy-monitor/PACKAGE-README.md)

The development setup for `tracy` uses the standard `bdep`-based workflow.
For example:

```
git clone .../tracy.git
cd tracy

bdep init -C @gcc cc config.cxx=g++
bdep update
bdep test
```
