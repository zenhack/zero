#include <kernel/x86/apic_setup.h>
#include <kernel/x86/apic_timer.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/x86/8259pic.h>

void ap_main(void) {
	remap_8259pic();
	disable_8259pic();
	apic_setup();
	apic_timer_init(APIC_TIMER_INT_NO, 7, APIC_TIMER_PERIODIC);
	apic_timer_set(apic_timer_init_count);
}
