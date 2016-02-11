#include <kernel/port/stdio.h>
#include <kernel/x86/apic_setup.h>

void ap_main(void) {
	apic_setup();
}
