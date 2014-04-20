#ifndef KERNEL_PANDA_MMIO_H
#define KERNEL_PANDA_MMIO_H

/* XXX: this is redundant with ../rpi/mmio.[hS]. Need to tweak the
 * build system to reduce duplication. */

/* Utility library for doing memory mapped io. */

#include <stdint.h>

/* These routines just load and store a 32-bit value from the address `addr`,
 * which must be 32-bit aligned. We use these for mmio rather than doing
 * simple assignment, since we don't trust the compiler to not break this kind of
 * code with optimizations. */
uint32_t get32(uint32_t addr);
void put32(uint32_t addr, uint32_t value);

#endif
