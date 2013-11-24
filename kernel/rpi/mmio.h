#ifndef KERNEL_RPI_MMIO_H
#define KERNEL_RPI_MMIO_H

/* Utility library for doing memory mapped io. */

#include <stdint.h>

#define IO_BASE 0x20000000

/* These routines just load and store a 32-bit value from the address `addr`,
 * which must be 32-bit aligned. We use these for mmio rather than doing
 * simple assignment, since we don't trust the compiler to break this kind of
 * code with optimizations. */
uint32_t get32(uint32_t addr);
void put32(uint32_t addr, uint32_t value);

#endif
