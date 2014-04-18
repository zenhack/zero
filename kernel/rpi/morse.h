#ifndef KERNEL_RPI_MORSE_H
#define KERNEL_RPI_MORSE_H

#include <stdint.h>

#define MORSE_UNIT 0x3f0000

void morse_write(char *s, uintptr_t len);
void sleep(uint32_t time);

#endif
