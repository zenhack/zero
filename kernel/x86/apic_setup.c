#include <kernel/port/panic.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/apic.h>

void apic_setup(void) {
	uint32_t local_apic_id;

	if(!have_apic()) {
		panic("No apic found!\n");
	}
	local_apic_id = get_local_apic_id();
	enable_local_apic();
	printf("Local Apic ID #%d is online.\n", local_apic_id);
}
