#ifndef KERNEL_RPI_MORSE_H
#define KERNEL_RPI_MORSE_H

#include <stddef.h>

#define MORSE_UNIT 0x3f0000

void morse_write(char *s, size_t len);

#endif
