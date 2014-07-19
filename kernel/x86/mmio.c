#include <kernel/x86/mmio.h>

uint32_t get32(uintptr_t addr) {
	return *(uint32_t*)addr;
}

void put32(uintptr_t addr, uint32_t value) {
	*(uint32_t*)addr = value;
}
