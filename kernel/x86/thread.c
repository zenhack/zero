#include <kernel/x86/thread.h>
#include <kernel/x86/gdt.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/panic.h>

#include <kernel/port/string.h>

typedef struct NewStack NewStack;
struct NewStack {
	Regs saved_ctx;
	uint32_t thread_ret;
	uint32_t thread_arg;
	uint32_t ebp_terminator;
};


Thread *mk_thread(size_t stack_size, void (*entry)(void *), void *data) {
	/* Below we construct a stack that looks like it's running in an
	 * interrupt handler, about to call int_handler_common, having been
	 * interrupted right after the call instruction invoking entry(data).
	 */

	void *stack = kalloc_align(stack_size, 4 * KIBI);
	Thread *ret = kalloc(sizeof(Thread));

	//debugging
	memset(stack, 0x12, stack_size);

	NewStack *stack_begin = (NewStack *)((size_t)stack - stack_size);

	stack_begin->saved_ctx.ds = SEGOFF(KDATA_SEGMENT);
	stack_begin->saved_ctx.ebp = (uint32_t)&stack_begin->ebp_terminator;
	/* We execute a pusha in isr_stub to save this; it would have been
	 * previously pointing at the thing beneath all the regs: */
	stack_begin->saved_ctx.esp = (uint32_t)&stack_begin->saved_ctx.int_no;
	/* put some pneumonic values here; we shouldn't rely on these after
	 * having returned from the interrupt handler: */
	stack_begin->saved_ctx.int_no = 0xbad1bad1;
	stack_begin->saved_ctx.error_code = 0xec0de000;

	stack_begin->saved_ctx.eip = (uint32_t)entry;
	stack_begin->saved_ctx.cs = SEGOFF(KCODE_SEGMENT);
	/* This is the on-boot value of the eflags register [intel/3/3.4.3]. We
	 * haven't done anything to modify it, so let's give new threads the
	 * same value: */
	stack_begin->saved_ctx.eflags = 0x2;
	stack_begin->thread_ret = 0;
	stack_begin->thread_arg = (uint32_t)data;
	stack_begin->ebp_terminator = 0;

	ret->ctx = &stack_begin->saved_ctx;
	ret->stack_end = stack;
	return ret;
}
