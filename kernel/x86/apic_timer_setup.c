
#include <kernel/port/sched.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/8259pic.h>
#include <kernel/x86/apic_timer.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/pit.h>

uint32_t apic_timer_frequency;
uint32_t apic_timer_init_count;


/* If we don't put volatile on these, the loop that checks if they've hit
 * appropriate values may be optimized to cache them in registers, which
 * obviously doesn't work. */
volatile uint32_t pit_ticks, apic_ticks;

static Regs *pit_calibrate_apic(Regs *old_ctx) {
	pit_ticks++;
	send_8259pic_EOI(0);
	return old_ctx;
}

static Regs *apic_calibrate_apic(Regs *old_ctx) {
	apic_ticks++;
	return old_ctx;
}

static Regs *apic_timer_sched(Regs *old_ctx) {
	return (Regs *)sched((void *)old_ctx);
}

void apic_timer_setup(uint32_t frequency) {
	int i;
	remap_8259pic();
	for(i = 0; i < 16; i++) {
		register_int_handler(IRQ(i), ignore_8259pic_irq);
	}
	register_int_handler(APIC_TIMER_INT_NO, apic_calibrate_apic);
	register_int_handler(IRQ(0), pit_calibrate_apic);
	apic_timer_init(APIC_TIMER_INT_NO, 7, APIC_TIMER_PERIODIC);

	printf("Measuring APIC timer frequency...\n");
	/* NOTE WELL: the arguments to these two functions are *not*
	 * conceputally the same; pit_init takes the frequency to set the clock
	 * to, while apic_timer_set takes an initial count.
	 *
	 * They're passed the same value so that the initial ratio between them
	 * is 1, and then we measure the resulting ratio of frequencies. */
	apic_timer_set(frequency);
	pit_init(frequency);

	sti();
	while(pit_ticks < 100) { hlt(); }
	cli();

	uint32_t new_init_count = (frequency * apic_ticks) / pit_ticks;

	printf("  APIC ticks: %d\n",     apic_ticks);
	printf("  PIT ticks: %d\n",      pit_ticks);
	printf("  ratio: %d\n",          apic_ticks/pit_ticks);
	printf("  new apic_start: %d\n", new_init_count);

	apic_timer_set(new_init_count);
	register_int_handler(IRQ(0), ignore_8259pic_irq);
	register_int_handler(APIC_TIMER_INT_NO, apic_timer_sched);

	disable_8259pic();
	apic_timer_frequency = frequency;
	apic_timer_init_count = new_init_count;
}
