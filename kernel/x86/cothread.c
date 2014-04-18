#include <kernel/x86/cothread.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <stddef.h>

/* defined in cothread_asm.S, along with  yield(). */
extern void *init_thread(void *stack, void (*entry)(void *));

void *mk_thread(size_t stack_size, void (*entry)(void *)) {
	/* align our stack on a page boundary. */
	void *stack = kalloc_align(stack_size, 4 * KIBI);
	if(!stack) {
		return NULL;
	}

	/* set up or thread, and return it. */
	return init_thread(stack, entry);
}
