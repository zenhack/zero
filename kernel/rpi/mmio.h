#ifndef KERNEL_RPI_MMIO_H
#define KERNEL_RPI_MMIO_H

/* Utility library for doing memory mapped io. */

#include <stdint.h>

#define IO_BASE 0x20000000

#define GPIO_BASE (IO_BASE + 0x00200000)

#define GPFSEL1 (GPIO_BASE + 4)
#define GPSET0 (GPIO_BASE + 28)
#define GPCLR0 (GPIO_BASE + 40)

/* These routines just load and store a 32-bit value from the address `addr`,
 * which must be 32-bit aligned. We use these for mmio rather than doing
 * simple assignment, since we don't trust the compiler to break this kind of
 * code with optimizations. */
uint32_t get32(uint32_t addr);
void put32(uint32_t addr, uint32_t value);

#endif
