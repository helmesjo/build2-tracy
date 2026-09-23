# tracy - <SUMMARY>

This is a `build2` package repository for [`tracy`](https://<UPSTREAM-URL>),
a <SUMMARY-OF-FUNCTIONALITY>.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`tracy` in your `build2`-based project, then instead see the accompanying
[`PACKAGE-README.md`](<PACKAGE>/PACKAGE-README.md) file.

The development setup for `tracy` uses the standard `bdep`-based workflow.
For example:

```
git clone .../tracy.git
cd tracy

bdep init -C @gcc cc config.cxx=g++
bdep update
bdep test
```
