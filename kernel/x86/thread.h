#ifndef KERNEL_X86_THREAD_H
#define KERNEL_X86_THREAD_H

#include <stddef.h>
#include <kernel/port/data.h>
#include <kernel/x86/regs.h>

typedef struct X86Thread X86Thread;
struct X86Thread {
	List l;
	Regs *ctx;
	void *stack_end;
};

X86Thread *mk_thread(size_t stack_size, void (*entry)(void *), void *data);

#endif
