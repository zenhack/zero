#ifndef KERNEL_X86_IDT_H
#define KERNEL_X86_IDT_H

#define NUM_IDT 256

typedef struct Regs Regs;

struct Regs {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, error_code;
	uint32_t eip, cs, eflags, useresp, ss;
}__attribute__((packed));

typedef void (*int_handler)(Regs *);

void idt_init(void);

#endif
