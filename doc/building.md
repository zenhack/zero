# Building

Short version:

	./configure --platform=...
	make

Where `...` is one of panda, rpi, x86, yeeloong.

You need to have a cross-toolchain to build zero, even if you're
building on the same CPU architecture. The toolchain should be
freestanding; you want to avoid linking against host libraries.

The configure script's options `--tool-prefix` and `--tool-suffix` can
be used to override the prefix/suffix of the toolchain; use this if the
configure script isn't guessing yours correctly. (This is likely; it
isn't being terribly smart about it).

Additionally, configure respects environment variables like `CC`, `LD`,
`OBJCOPY`...

Out of tree builds are supported as well, and are recommended:

	mkdir ../zero-build
	cd ../zero-build
	../zero/configure --platform=...
	make

Please note that building within a subdirectory is *not* supported, and
it is very likely that doing this will go badly for you.

## Extra targets for x86

There are a handful of useful extra targets for the x86 platform:

* `boot.iso`: A bootable ISO image which can be written to CDs, USB
  sticks, hard drives etc. Grub 2 must be installed on your system in
  order to build this.
* `qemu-run`, `bochs-run`: run the kernel under qemu or bochs. The
  `bochs-run` target depends on `boot.iso`. `qemu-run` uses qemu's
  native multiboot support to boot the kernel directly.
* `qemu-gdb`: Like `qemu-run`, but waits for a remote debugging
  connection from gdb.
