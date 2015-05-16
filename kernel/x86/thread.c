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
	ret->regs.esp = (uint32_t)&stack[1];
	ret->regs.ebp = 0;
	ret->regs.eip = (uint32_t)entry;
	stack[0] = (uint32_t)data;
	stack[1] = 0; /* This is the ret address, but threads should *never* return. */
	ret->regs.ds = SEGOFF(KDATA_SEGMENT);
	return ret;
}
