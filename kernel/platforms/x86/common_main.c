#include <kernel/port/stdio.h>
#include <kernel/port/sched.h>
#include <kernel/x86/regs.h>
#include <kernel/x86/thread.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/x86/asm.h>

static void test_thread(void *data) {
	uint32_t thread_id = (uint32_t)data;
	while(1) {
		uint32_t apic_id = get_local_apic_id();
		printf("Thread %d running on apic %d\n", thread_id, apic_id);
	}
}

static Regs *sched_int_handler(Regs *old_ctx) {
	return (Regs *)sched((void *)old_ctx);
}

void common_main(void) {
	int32_t apic_id = get_local_apic_id();
	X86Thread *mythread = mk_thread(test_thread, (void*)apic_id);
	sched_insert((Thread *)mythread);
	/* XXX: this is a bit sketchy; all the cpus are sharing an idt, so
	 * they're *all* going to do this. It *should* be okay, since they're
	 * all setting it to the same value, but it's pretty gross. */
	register_int_handler(APIC_TIMER_INT_NO, sched_int_handler);
	sti();
	while(1) {
		hlt();
	}
}
