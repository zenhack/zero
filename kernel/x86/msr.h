#ifndef KERNEL_X86_MSR_H
#define KERNEL_X86_MSR_H

#include <stdint.h>

static inline
uint64_t rdmsr(uint32_t msr) {
	uint32_t ret_lo, ret_hi;
	asm ("rdmsr" : "=a"(ret_lo), "=d"(ret_hi) : "c"(msr));
	return ret_lo | ((uint64_t)ret_hi<<32);
}

static inline
void wrmsr(uint32_t msr, uint64_t value) {
	uint32_t lo, hi;
	hi = value>>32;
	lo = value;
	asm ("wrmsr" :: "c"(msr), "a"(lo), "d"(hi));
}

#endif
