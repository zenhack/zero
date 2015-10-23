
#include <kernel/port/sched.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/8259pic.h>
#include <kernel/x86/apic_timer.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/pit.h>

uint32_t apic_timer_frequency;

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
	register_int_handler(255, apic_calibrate_apic);
	register_int_handler(IRQ(0), pit_calibrate_apic);
	apic_timer_init(255, 7, APIC_TIMER_PERIODIC);

	printf("Measuring APIC timer frequency...\n");
	apic_timer_set(frequency);
	pit_init(frequency);

	sti();
	while(pit_ticks < 100) { hlt(); }
	cli();

	uint32_t new_init_count = (frequency * pit_ticks) / apic_ticks;

	printf(
		"  APIC ticks: %d\n"
		"  PIT ticks: %d\n"
		"  ratio: %d\n"
		"  new apic_start: %d\n",

		pit_ticks,
		apic_ticks,
		apic_ticks/pit_ticks,
		new_init_count
	);

	apic_timer_set(new_init_count);
	register_int_handler(IRQ(0), ignore_8259pic_irq);
	register_int_handler(255, apic_timer_sched);

	disable_8259pic();
	apic_timer_frequency = frequency;
}
