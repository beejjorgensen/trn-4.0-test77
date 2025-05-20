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

## Building

If you're building for a local install in `~/.local/bin` on a Mac or
Linux or you're doing an [AUR](https://aur.archlinux.org/packages/trn)
build, you can run `./configbot` in the `src/` directory. This is just a
front-end to various `configbot.*` scripts. *This uses the `expect`
tool, so have that installed.*

If you want to do the classic interactive configure, run `./Configure`
instead.

1. Run `./configbot` (automatic) or `./Configure` (manual)
2. `make`
3. `make install`

## Running

Environment variables:

```
export FROM='Me <me@example.com>'
export ORGANIZATION="Yoyodyne Propulsion Systems"
export NEWSSIGNATURE=~/.trn/signature
export VISUAL=vim
export EDITOR="$VISUAL"
```

Add a `~/.trn/access` file, mode `0600` since it has a password in the
clear. Here's one for
[`eternal-september.org`](https://www.eternal-september.org/):

```
[default]
NNTP Server = news.eternal-september.org
Auth User = your_username
Auth Password = your_password
Force Auth = yes
```

The `Force Auth` is required for some servers, apparently. And also,
apparently, you can add multiple NNTP servers to the `access` file and
cycle between them.

## TODO

* `sighold` and `sigrelse` are deprecated. But no one is warning... yet!
* `egrep` in `Configure` is deprecated.
