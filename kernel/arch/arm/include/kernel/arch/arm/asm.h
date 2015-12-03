#ifndef KERNEL_ARCH_ARM_ASM_H
#define KERNEL_ARCH_ARM_ASM_H

#include <stdint.h>

static inline
uint32_t ldrex(uint32_t *addr) {
	uint32_t ret;
	__asm__ volatile(
		"ldrex %[ret], [%[addr]]\n"
		: [ret] "=r"(ret)
		: [addr] "r"(addr)
	);
	return ret;
}

static inline
uint32_t strex(uint32_t *addr, uint32_t value) {
	uint32_t ret;
	__asm__ volatile(
		"strex %[ret], %[value], [%[addr]]\n"
		/* The '&' constraint stands for "early clobber." This
		 * prevents the compiler from using the same register as one of
		 * the inputs, which is invalid. */
		: [ret] "=&r"(ret)
		: [value] "r"(value), [addr] "r"(addr)
	);
	return ret;
}

#endif
