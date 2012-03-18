#ifndef KERNEL_PORT_STDIO_H
#define KERNEL_PORT_STDIO_H

#include <stdint.h>

typedef struct FILE FILE;
struct FILE {
	uintptr_t (*write)(FILE *, void *, uintptr_t);
	void *aux;
};

extern FILE *stdout;

int fprintf(FILE *stream, char *fmt, ...);

int printf(char *fmt, ...);

#endif
