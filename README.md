# TRN the threaded newsreader

This repo is here for two reasons.

1. History, in case it goes away from [its home on
   SourceForge](https://trn.sourceforge.net/) (or if SourceForge goes
   away!)

2. Modernizing—I might try to make the minimum modifications necessary
   to get it to build correctly under Linux.

But if you're _really_ interested in modernized trn, you should see the
[trn 5.0](https://github.com/LegalizeAdulthood/trn) project which is
meant to be used for realsies. This repo is just trying to mitigate rot.
Or, as they call it in the ghost town of Bodie, I'm keeping it in a
state of arrested decay.

The only thing I'm really building for is (Arch) Linux, but I'd gladly
take PRs that made it work on other platforms.

## TODO

* All function definitions need to be ANSI-fied.
* Everything to do with signals is deprecated.
* `term.h` should include `<term.h>` to get `tputs()`, but that breaks a
  lot of things.
