#include <kernel/port/stdio.h>
#include <kernel/port/mmio.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/regs.h>
#include <kernel/x86/apic_timer_setup.h>

static volatile uint32_t delay_counter;

static Regs *increment_delay_counter(Regs *old_ctx) {
	delay_counter++;
	return old_ctx;
}


/** Wait `delay_ms` milliseconds. This has the side effect of overriding the
 * apic timer interrupt handler with `increment_delay_counter`. As such, it's
 * not really suitable for use outside of mp_setup. */
static void wait_ms(uint32_t delay_ms) {
	uint32_t delay_ticks = (delay_ms * apic_timer_frequency) / 1000;
	delay_counter = 0;
	register_int_handler(APIC_TIMER_INT_NO, increment_delay_counter);
	sti();
	while(delay_counter < delay_ticks) {
		hlt();
	}
}

void mp_setup(void) {
	while(1) {
		printf("Tick\n");
		wait_ms(10);
	}
# if 0
	/** test sending IPIs: */
	ApicICR icr;
	icr.lo.raw = get32(INT_COMMAND);

	icr.lo.dest_shorthand = ICR_IPI_ALL_BUT_SELF;
	icr.lo.deliv_mode = ICR_DELIV_INIT;
	icr.lo.level = ICR_ASSERT;
	put32(INT_COMMAND, icr.lo.raw);

	sti();
#endif
}
