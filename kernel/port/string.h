#ifndef KERNEL_PORT_STRING_H
#define KERNEL_PORT_STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(char *s);

int strncmp(char *s1, char *s2, size_t len);

int strcmp(char *s1, char *s2);

char *strchr(char *s, int c);

void *memcpy(void *dest, const void *src, size_t len);

void *memset(void *s, int c, size_t n);

#endif
