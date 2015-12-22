#include <kernel/port/stdio.h>
#include <kernel/port/mmio.h>
#include <kernel/port/panic.h>
#include <kernel/x86/acpi.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/regs.h>
#include <kernel/x86/apic_timer_setup.h>


static size_t get_cpu_count(void) {
	/* TODO: move more of this logic to acpi.c */
	acpi_RSDP *rsdp = acpi_find_rsdp();
	if(!rsdp) {
		panic("RSDP not found!\n");
	}

	acpi_SDT *rsdt = (acpi_SDT*)rsdp->v1.rsdt_addr;
	if(!acpi_verify_sdt(rsdt)) {
		panic("RSDT checksum invalid!\n");
	}
	size_t num_sdts = (rsdt->header.length - sizeof(acpi_SDTHeader)) / sizeof(acpi_SDT *);
	acpi_SDT **sdts = (acpi_SDT**)&rsdt->body.rsdt.sdts;
	for(size_t i = 0; i < num_sdts; i++) {
		if(!acpi_verify_sdt(sdts[i])) {
			char *sig = (char *)&sdts[i]->header.signature[0];
			printf("Bad checksum for SDT %d (signature \"%c%c%c%c\").\n",
				i, sig[0], sig[1], sig[2], sig[3]);
			panic("Bad SDT checksum!\n");
		}
	}
	/* TODO: finish writing this; stopping to test. */
	return num_sdts;
}

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
	printf("%d\n", get_cpu_count());
	while(1) {
//		printf("Tick\n");
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