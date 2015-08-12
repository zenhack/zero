#ifndef KERNEL_X86_ASM_H
#define KERNEL_X86_ASM_H

/** Thin wrappers around x86 assembly instructions. */

#include <stdint.h>

static inline void cli(void) { __asm__ volatile("cli"); }
static inline void hlt(void) { __asm__ volatile("hlt"); }

static inline
uint64_t rdmsr(uint32_t msr) {
	uint32_t ret_lo, ret_hi;
	__asm__ ("rdmsr" : "=a"(ret_lo), "=d"(ret_hi) : "c"(msr));
	return ret_lo | ((uint64_t)ret_hi<<32);
}

static inline void sti(void) { __asm__ volatile("sti"); }

static inline
void wrmsr(uint32_t msr, uint64_t value) {
	uint32_t lo, hi;
	hi = value>>32;
	lo = value;
	__asm__ ("wrmsr" :: "c"(msr), "a"(lo), "d"(hi));
}

static inline
uint32_t cmpxchg(uint32_t eax, uint32_t *dest, uint32_t src) {
	__asm__ volatile(
		"lock\n"
		"cmpxchg %d[src], %d[dest]\n"
		: "=a"(eax), "=m"(*dest)
		: "a"(eax), [dest] "m"(*dest), [src] "r"(src)
	);
	return eax;
}

#endif
