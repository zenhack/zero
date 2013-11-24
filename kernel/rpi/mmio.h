#ifndef KERNEL_RPI_MMIO_H
#define KERNEL_RPI_MMIO_H

/* Utility library for doing memory mapped io. */

#include <stdint.h>

#define IO_BASE 0x20000000

#define GPIO_BASE (IO_BASE + 0x00200000)

#define GPFSEL1 (GPIO_BASE + 4)
#define GPSET0 (GPIO_BASE + 28)
#define GPCLR0 (GPIO_BASE + 40)

#define AUX_BASE (IO_BASE + 0x215000)

#define AUX_ENABLES (AUX_BASE + 0x4)
#define AUX_MU_IO_REG (AUX_BASE + 0x40)
#define AUX_MU_IER_REG (AUX_BASE + 0x44)
#define AUX_MU_IIR_REG (AUX_BASE + 0x48)
#define AUX_MU_LCR_REG (AUX_BASE + 0x4c)
#define AUX_MU_MCR_REG (AUX_BASE + 0x50)
#define AUX_MU_LSR_REG (AUX_BASE + 0x54)
#define AUX_MU_MSR_REG (AUX_BASE + 0x58)
#define AUX_MU_SCRATCH (AUX_BASE + 0x5c)
#define AUX_MU_CNTL_REG (AUX_BASE + 0x60)
#define AUX_MU_STAT_REG (AUX_BASE + 0x64)
#define AUX_MU_BAUD_REG (AUX_BASE + 0x68)
#define AUX_SPI0_CNTL0_REG (AUX_BASE + 0x80)
#define AUX_SPI0_CNTL1_REG (AUX_BASE + 0x84)
#define AUX_SPI0_STAT_REG (AUX_BASE + 0x88)
#define AUX_SPI0_IO_REG (AUX_BASE + 0x90)
#define AUX_SPI0_PEEK_REG (AUX_BASE + 0x94)
#define AUX_SPI1_CNTL0_REG (AUX_BASE + 0xc0)
#define AUX_SPI1_CNTL1_REG (AUX_BASE + 0xc4)
#define AUX_SPI1_STAT_REG (AUX_BASE + 0xc8)
#define AUX_SPI1_IO_REG (AUX_BASE + 0xd0)
#define AUX_SPI1_PEEK_REG (AUX_BASE + 0xd4)

/* These routines just load and store a 32-bit value from the address `addr`,
 * which must be 32-bit aligned. We use these for mmio rather than doing
 * simple assignment, since we don't trust the compiler to break this kind of
 * code with optimizations. */
uint32_t get32(uint32_t addr);
void put32(uint32_t addr, uint32_t value);

#endif
