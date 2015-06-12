#include <kernel/x86/thread.h>
#include <kernel/x86/gdt.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/panic.h>

Thread *mk_thread(size_t stack_size, void (*entry)(void *), void *data) {
	uint32_t *stack = kalloc_align(stack_size, 4 * KIBI);
	Thread *ret = kalloc(sizeof(Thread));
	if(!stack || !ret) {
		if(stack) kfree(stack, stack_size);
		if(ret) kfree(ret, sizeof(Thread));
		return NULL;
	}
	stack = (uint32_t*)(((size_t)stack) + stack_size);
	stack[-1] = 0; /* end of the ebp "linked list" for stack traces. */
	stack[-2] = (uint32_t)data;
	stack[-3] = 0; /* This is the ret address, but threads should *never* return. */
	ret->regs.ebp = (uint32_t)&stack[-2];
	/* useresp is the stack pointer for the interrupted process;
	 * esp is the stack pointer as saved mid-interrupt handler, and
	 * thus *not* what we want to point at our new stack. */
	ret->regs.useresp = (uint32_t)&stack[-4];

	ret->regs.eip = (uint32_t)entry;
	ret->regs.ds = SEGOFF(KDATA_SEGMENT);
	ret->regs.ss = SEGOFF(KDATA_SEGMENT);
	ret->regs.cs = SEGOFF(KCODE_SEGMENT);

	/* The first time we're scheduled, the scheduler will fill these
	 * in for us, but we need to clearly mark them as uninitialized: */
	ret->regs.esp = 0;
	ret->regs.eflags = 0;

	return ret;
}
