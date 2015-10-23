#include <kernel/port/stdio.h>
#include <kernel/port/mmio.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/regs.h>

static Regs *test_show_local_apic_id(Regs *old_ctx) {
	printf("%d\n", get_local_apic_id());
	return old_ctx;
}

void mp_setup(void) {
	/** test sending IPIs: */
	register_int_handler(254, test_show_local_apic_id);
	ApicICR icr;
	icr.lo.raw = get32(INT_COMMAND);

	icr.lo.dest_shorthand = ICR_IPI_ALL_BUT_SELF;
	icr.lo.deliv_mode = ICR_DELIV_INIT;
	icr.lo.level = ICR_ASSERT;
	put32(INT_COMMAND, icr.lo.raw);

	sti();
}
