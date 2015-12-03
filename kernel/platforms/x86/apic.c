#include <kernel/x86/apic.h>
#include <kernel/x86/cpuid.h>
#include <kernel/port/mmio.h>

int have_apic(void) {
	CPUInfo cpu_info;
	cpu_info.eax = 1;
	cpuid(&cpu_info);
	return cpu_info.edx & HAVE_APIC_FLAG;
}

void enable_local_apic(void) {
	put32(SPUR_INT_VEC, get32(SPUR_INT_VEC) | APIC_SOFTWARE_ENABLE);
}

uint32_t get_local_apic_id(void) {
	return get32(LOCAL_APIC_ID);
}
