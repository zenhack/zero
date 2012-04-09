#ifndef KERNEL_X86_CPUID_H
#define KERNEL_X86_CPUID_H

#include <stdint.h>

typedef struct CPUInfo CPUInfo;

struct CPUInfo {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
}__attribute__((packed));

void cpuid(CPUInfo *info);

/* feature flags */

/* if eax = 1 */
#define HAVE_APIC_FLAG (1<<9)

#endif

