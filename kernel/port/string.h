#ifndef KERNEL_PORT_STRING_H
#define KERNEL_PORT_STRING_H

#include <stdint.h>
#include <stddef.h>

uintptr_t strlen(char *s);

void *memcpy(void *dest, const void *src, size_t len);

void *memset(void *s, int c, size_t n);

#endif
