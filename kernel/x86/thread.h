#ifndef KERNEL_X86_THREAD_H
#define KERNEL_X86_THREAD_H

#include <stddef.h>
#include <kernel/x86/regs.h>

typedef struct Thread Thread;
struct Thread {
	Thread *next;
	Regs regs;
};

Thread *mk_thread(size_t stack_size, void (*entry)(void *), void *data);

#endif