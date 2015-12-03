#include <kernel/x86/thread.h>
#include <kernel/x86/gdt.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/panic.h>

#include <kernel/port/string.h>

/* See [intel/1/3.4.3] */
#define EFLAGS_ALWAYS1 (1<<1)
#define EFLAGS_IF (1<<9)

#define STACK_SIZE (8 * KIBI)

typedef struct NewStackBase NewStackBase;
typedef struct NewStack NewStack;

struct NewStackBase {
	Regs saved_ctx;
	uint32_t thread_ret;
	uint32_t thread_arg;
	uint32_t ebp_terminator;
};

struct NewStack {
	uint8_t extra_space[STACK_SIZE - sizeof(NewStackBase)];
	NewStackBase base;
};

_Static_assert(sizeof(NewStack) == STACK_SIZE, "NewStack struct is the wrong size");


X86Thread *mk_thread(void (*entry)(void *), void *data) {
	/* Below we construct a stack that looks like it's running in an
	 * interrupt handler, about to call int_handler_common, having been
	 * interrupted right after the call instruction invoking entry(data).
	 */

	NewStack *stack = kalloc_align(sizeof(NewStack), 4 * KIBI);
	X86Thread *ret = kalloc(sizeof(X86Thread));

	/* 0xcc is a breakpoint instruction (int $3). If we accidently jump into
	 * the stack (yay memory saftey :\), this will increase the probability
	 * that we hear about it early. */
	memset(stack, 0xcc, STACK_SIZE);

	NewStackBase *base = &stack->base;
	base->saved_ctx.ds = SEGOFF(KDATA_SEGMENT);
	base->saved_ctx.ebp = (uint32_t)&base->ebp_terminator;
	/* We execute a pusha in isr_stub to save this; it would have been
	 * previously pointing at the thing beneath all the regs: */
	base->saved_ctx.esp = (uint32_t)&base->saved_ctx.int_no;
	/* put some pneumonic values here; we shouldn't rely on these after
	 * having returned from the interrupt handler: */
	base->saved_ctx.int_no = 0xbad1bad1;
	base->saved_ctx.error_code = 0xec0de000;

	base->saved_ctx.eip = (uint32_t)entry;
	base->saved_ctx.cs = SEGOFF(KCODE_SEGMENT);
	/* The on-boot value of eflags just has the one reserved bit set. The
	 * only modification we've made is to enable interrupts
	 * [intel/1/3.4.3]: */
	base->saved_ctx.eflags = EFLAGS_ALWAYS1 | EFLAGS_IF;
	/* the return address should never actually be used; threads are not
	 * permitted to return: */
	base->thread_ret = 0;
	base->thread_arg = (uint32_t)data;
	base->ebp_terminator = 0;

	ret->ctx = &base->saved_ctx;
	ret->stack_end = stack;
	return ret;
}
