#include <kernel/port/stdio.h>
#include <kernel/x86/apic.h>

void ap_main(void) {
	printf("Hello from apic #%d!\n", get_local_apic_id());
}
