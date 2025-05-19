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

## Config scripts

I've included some `expect`-driven scripts that run the extremely-manual
`./Configure` script.

* `configbot.mac.local`: sets up for a Mac build and local install in
  `~/.local/bin` (so make sure that's in your `PATH`.
* `configbot.archlinux`: sets up for a Linux build with installation in
  the system directories. Basically an AUR builder.

## Building

1. Run one of the above bot scripts or `./Configure` to do it manually.
2. `make`
3. `make install`

## Running

Environment variables:

```
export NNTPSERVER=news.eternal-september.org  # For example
export ORGANIZATION="Yoyodyne"
export VISUAL=vim
export EDITOR="$VISUAL"
export FROM='Me <me@example.com>'
export SIGNATURE_FILE=~/.sig
```

If you need authorization, try:

```
export NNTP_FORCE_AUTH=YES  # Force trn to use ~/.nntpauth
```

And make a `~/.nntpauth` file. In that file have two lines:

```
loginname
password
```

`chmod 600 ~/.nntpauth` so that others can't read it.

(There might be a better way to do auth—I'm slowly figuring it out.)

Then `trn` and you're off.

## Auth issues

https://web.archive.org/web/20220629054850/https://www.foxvalley.net/nntpsetup.html

```
trn
Using a text editor, create a file in your .trn directory called "access". The access file should be formatted like this:
[default]
NNTP Server = news.foxvalley.net
Auth User = your_email_address
Auth Password = your_password
```

Maybe this is the _Right Way_ and I need to remove that code that protects
`.nntpauth`. That would make me happy.

Read: `HelpFiles/samples/access`

I can't tell that the `~/.trn/access` file is actually read anywhere. At
least for `eternal-september`, it seems to be ignored and I have to use
`.nntpauth`.

## TODO

* Set up more expect scripts for different configurations.
* `sighold` and `sigrelse` are deprecated. But no one is warning... yet!
* Fix auth properly?
