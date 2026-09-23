# libtracy-client - Tracy Profiler

This is a `build2` package for the [`<UPSTREAM-NAME>`](https://<UPSTREAM-URL>)
C++ library. It provides <SUMMARY-OF-FUNCTIONALITY>.


## Usage

To start using `libtracy-client` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libtracy-client ^<VERSION>
```

Then import the library in your `buildfile`:

```
import libs = libtracy-client%lib{<TARGET>}
```


## Importable targets

This package provides the following importable targets:

```
lib{<TARGET>}
```

<DESCRIPTION-OF-IMPORTABLE-TARGETS>


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libtracy_client.<VARIABLE> ?= false
```

<DESCRIPTION-OF-CONFIG-VARIABLES>
