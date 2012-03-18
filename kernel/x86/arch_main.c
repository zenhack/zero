#include <kernel/x86/multiboot.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/serial.h>
#include <kernel/port/units.h>

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void arch_main(MultiBootInfo *mb_info) {
	FILE com1;
	serial_init(COM1, &com1);
	stdout = &com1;
	printf("Hello, World!\n");
	printf("Mboot info at : 0x%x, high-memory: 0x%x\n", mb_info, mb_info->mem_upper * KIBI);
	printf("kernel end at : 0x%x", &kend);
	while(1);
}
