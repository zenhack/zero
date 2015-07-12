#ifndef KERNEL_X86_REGS_H
#define KERNEL_X86_REGS_H

#include <stdint.h>

typedef struct Regs Regs;

/* Saved state upon entering an interrupt handler */
struct Regs {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no;

	/* x86 CPUs only push error codes for *some* interrupts; in other cases
	 * this will be a dummy value provided by us: */
	uint32_t error_code;

	/* everything from here down is pushed by the CPU. */
	uint32_t eip, cs, eflags;

	/* These two only exist if the interrupted task was running in user
	 * mode. Otherwise, we're running on the same stack. */
	uint32_t useresp, ss;
}__attribute__((packed));

#endif
