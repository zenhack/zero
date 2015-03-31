#include <kernel/port/mmio.h>

uint32_t get32(uintptr_t addr) {
	volatile uint32_t *vaddr = (uint32_t*)addr;
	return *vaddr;
}

void put32(uintptr_t addr, uint32_t value) {
	volatile uint32_t *vaddr = (uint32_t*)addr;
	*vaddr = value;
}
