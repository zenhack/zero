#include <kernel/x86/multiboot.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/serial.h>
#include <kernel/port/units.h>
#include <kernel/port/heap.h>
#include <kernel/port/string.h>
#include <kernel/x86/gdt.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/cpuid.h>

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void breakpoint_handler(Regs *regs) {
	printf("A wild breakpoint appeared!\n");
}

void arch_main(MultiBootInfo *mb_info) {
	FILE com1;
	CPUInfo cpu_info;
	char cpu_vendor[13];
	int i;
	gdt_init();
	serial_init(COM1, &com1);
	stdout = &com1;
	idt_init();
	register_int_handler(0x3, breakpoint_handler);
	asm volatile("int $0x3");
	printf("Hello, World!\n");
	printf("Mboot info at : 0x%x, high-memory: 0x%x\n", mb_info, mb_info->mem_upper * KIBI);
	printf("kernel end at : 0x%x\n", &kend);
	heap_init((uintptr_t)&kend, (uintptr_t)mb_info->mem_upper * KIBI);
	
	for(i = 0; i < 1000; i++) {
		char *x, *y, *z;
		x = kalloc(512);
		y = kalloc(512);
		z = kalloc(512);
		if(!(x && y && z)) {
			printf("Out of memory!\n");
		}
		kfree(x, 512);
		kfree(z, 512);
		kfree(y, 512);
	}
	printf("Ok!\n");
	memset(&cpu_info, 0, sizeof(CPUInfo));
	cpuid(&cpu_info);
	memcpy(&cpu_vendor[0], &cpu_info.ebx, sizeof(uint32_t));
	memcpy(&cpu_vendor[4], &cpu_info.edx, sizeof(uint32_t));
	memcpy(&cpu_vendor[8], &cpu_info.ecx, sizeof(uint32_t));
	cpu_vendor[12] = '\0';
	printf("Cpu: %s\n", cpu_vendor);
	while(1);
}
