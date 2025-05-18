# TRN the threaded newsreader

This repo is here for two reasons.

1. History, in case it goes away from [its home on
   SourceForge](https://trn.sourceforge.net/) (or if SourceForge goes
   away!)

2. Modernizing—All the old K&R1 stuff has been updated to ANSI, and a
   variety of warnings and build errors have been fixed. The intention
   was that there would be no logic changes.

Currently builds on:

* Linux: Clang and GCC
* Mac: Clang

Maybe more, but that's all I have.

If you're _really_ interested in modernized trn, you should see the [trn
5.0](https://github.com/LegalizeAdulthood/trn) project which is meant to
be used for realsies. This repo is just trying to mitigate rot. Or, as
they call it in the ghost town of Bodie, I'm keeping it in a state of
arrested decay.

## TODO

* Set up more expect scripts for different configurations.
* When posting news, how to set the From?
* When posting news, the `nntpauth` file gets deleted...?
