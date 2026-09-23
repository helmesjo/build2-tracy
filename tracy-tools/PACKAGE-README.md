# tracy-tools - Tracy Profiler command line tools

This is a `build2` package for the [`tracy`](https://github.com/wolfpld/tracy)
command line tools. They record and process traces of Tracy, a real time,
nanosecond resolution, remote telemetry, hybrid frame and sampling profiler
for games and other applications: `tracy-capture` and
`tracy-capture-daemon` record traces from instrumented programs (see the
`libtracy-client` package), `tracy-csvexport` exports zone statistics as
CSV, `tracy-update` converts and recompresses traces, `tracy-merge` merges
traces, and `tracy-import-chrome` and `tracy-import-fuchsia` convert
Chrome and Fuchsia traces.


## Usage

To start using `tracy-tools` in your project, add the following build-time
`depends` value to your `manifest`, adjusting the version constraint as
appropriate:

```
depends: * tracy-tools ^0.14.1
```

Then import the executable in your `buildfile`:

```
import tracy_capture = tracy-tools%exe{tracy-capture}
```


## Importable targets

This package provides the following importable targets:

```
exe{tracy-capture}
exe{tracy-capture-daemon}
exe{tracy-csvexport}
exe{tracy-import-chrome}
exe{tracy-import-fuchsia}
exe{tracy-update}
exe{tracy-merge}
```

The Tracy command line tools described above.


## Configuration variables

This package provides no configuration variables.
