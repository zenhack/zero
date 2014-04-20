#ifndef KERNEL_X86_HLT_H
#define KERNEL_X86_HLT_H

static inline
void hlt(void) {
	asm volatile("hlt");
}

#endif
