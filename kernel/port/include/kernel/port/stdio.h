#ifndef KERNEL_PORT_STDIO_H
#define KERNEL_PORT_STDIO_H

#include <stddef.h>

/**
 * A FILE object is a stream to which data can be written (and, perhaps,
 * down the line read from).
 *
 * Different sorts of objects will declare types with this as their
 * first member, and thus pointers to those types are also pointers to
 * FILEs. write should be a function which writes the data in `buf`
 * (which will be of size `len` bytes) to the file object (which will
 * always be passed in as `out`. Typically, the function will be
 * customized to the type in which the file object is embedded.
 */
typedef struct FILE FILE;
struct FILE {
	size_t (*write)(FILE *out, void *buf, size_t len);
};

extern FILE *stdout;

int fprintf(FILE *stream, char *fmt, ...);

int printf(char *fmt, ...);

#endif
