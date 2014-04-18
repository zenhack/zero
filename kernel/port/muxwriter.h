#ifndef KERNEL_PORT_MUXWRITER_H
#define KERNEL_PORT_MUXWRITER_H

#include <kernel/port/stdio.h>

/**
 * A MuxWriter is a FILE object which allows for multiplexing output
 * between two (other) separate FILE objects.
 */
typedef struct MuxWriter MuxWriter;
struct MuxWriter {
	size_t (*write)(FILE *, void *, size_t);
	FILE *files[2];
};

void muxwriter_init(MuxWriter *, FILE *, FILE *);

#endif
