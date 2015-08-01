#include <kernel/x86/thread.h>
#include <kernel/x86/gdt.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/panic.h>

#include <kernel/port/string.h>

/* See [intel/1/3.4.3] */
#define EFLAGS_ALWAYS1 (1<<1)
#define EFLAGS_IF (1<<9)


typedef struct NewStack NewStack;
struct NewStack {
	Regs saved_ctx;
	uint32_t thread_ret;
	uint32_t thread_arg;
	uint32_t ebp_terminator;
};


X86Thread *mk_thread(size_t stack_size, void (*entry)(void *), void *data) {
	/* Below we construct a stack that looks like it's running in an
	 * interrupt handler, about to call int_handler_common, having been
	 * interrupted right after the call instruction invoking entry(data).
	 */

	void *stack = kalloc_align(stack_size, 4 * KIBI);
	X86Thread *ret = kalloc(sizeof(X86Thread));

	/* 0xcc is a breakpoint instruction (int $3). If we accidently jump into
	 * the stack (yay memory saftey :\), this will increase the probability
	 * that we hear about it early. */
	memset(stack, 0xcc, stack_size);

	NewStack *stack_begin = (NewStack *)((size_t)stack + stack_size);
	stack_begin--;

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
	/* The on-boot value of eflags just has the one reserved bit set. The
	 * only modification we've made is to enable interrupts
	 * [intel/1/3.4.3]: */
	stack_begin->saved_ctx.eflags = EFLAGS_ALWAYS1 | EFLAGS_IF;
	stack_begin->thread_ret = 0;
	stack_begin->thread_arg = (uint32_t)data;
	stack_begin->ebp_terminator = 0;

	ret->ctx = &stack_begin->saved_ctx;
	ret->stack_end = stack;
	return ret;
}
