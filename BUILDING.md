# Building

Short version:

	./configure --platform=...
	make

Where ... is one of panda, rpi, x86.

The options --tool-prefix and --tool-suffix can be used to override the
prefix/suffix of the toolchain; use this if the configure script isn't
guessing your correctly. (This is likely; it isn't being terribly smart
about it).

Additionally, configure respects environment variables like CC, LD,
OBJCOPY...

Out of tree builds are supported as well, and are recommended:

	mkdir ../zero-build
	cd ../zero-build
	../zero/configure --platform=...
	make
