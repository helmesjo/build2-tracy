# tracy-profiler - Tracy Profiler GUI

This is a `build2` package for the [`tracy`](https://github.com/wolfpld/tracy)
`tracy-profiler` executable. It is the Tracy profiler application, which
connects to instrumented applications or opens saved traces and displays
their frame, zone, sampling, memory, and other profiling data.

Unlike upstream, this package always uses the GLFW backend, including on
Linux (X11), where upstream uses its native Wayland backend by default.


## Usage

To start using `tracy-profiler` in your project, add the following build-time
`depends` value to your `manifest`, adjusting the version constraint as
appropriate:

```
depends: * tracy-profiler ^0.14.1
```

Then import the executable in your `buildfile`:

```
import tracy_profiler = tracy-profiler%exe{tracy-profiler}
```


## Importable targets

This package provides the following importable targets:

```
exe{tracy-profiler}
```

The Tracy profiler application.


## Configuration variables

This package provides no configuration variables.
