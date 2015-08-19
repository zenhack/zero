#ifndef KERNEL_X86_IDT_H
#define KERNEL_X86_IDT_H

#include <kernel/x86/regs.h>

#define NUM_IDT 256

typedef Regs *(*int_handler)(Regs *);

void register_int_handler(uint8_t num, int_handler h);

void idt_init(void);

#endif
