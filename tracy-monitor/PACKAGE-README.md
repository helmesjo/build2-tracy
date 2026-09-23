# tracy-monitor - Tracy Profiler monitor for external processes

This is a `build2` package for the [`tracy`](https://github.com/wolfpld/tracy)
`tracy-monitor` executable. It collects sampling profiling data from
another process (launched by it or attached to by PID) and makes it
available to a Tracy server, such as `tracy-capture` from the `tracy-tools`
package or the Tracy profiler application. It is only supported on Linux.

Unlike upstream, this package uses the `libbacktrace` package instead of
Tracy's modified copy, which cannot resolve symbols of the monitored
process, so its frames are reported as addresses.


## Usage

To start using `tracy-monitor` in your project, add the following build-time
`depends` value to your `manifest`, adjusting the version constraint as
appropriate:

```
depends: * tracy-monitor ^0.14.1
```

Then import the executable in your `buildfile`:

```
import tracy_monitor = tracy-monitor%exe{tracy-monitor}
```


## Importable targets

This package provides the following importable targets:

```
exe{tracy-monitor}
```

The Tracy external process monitor.


## Configuration variables

This package provides no configuration variables.
