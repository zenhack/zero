#include <kernel/x86/multiboot.h>
#include <kernel/port/stdio.h>
#include <kernel/port/muxwriter.h>
#include <kernel/x86/serial.h>
#include <kernel/port/units.h>
#include <kernel/port/heap.h>
#include <kernel/port/string.h>
#include <kernel/port/panic.h>
#include <kernel/x86/gdt.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/cpuid.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/text_console.h>
#include <kernel/x86/cothread.h>
#include <kernel/x86/paging.h>

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void breakpoint_handler(Regs *regs) {
	printf("A wild breakpoint appeared!\n");
}

void thread1(void *other) {
	while(1) {
		printf("Thread 1 !!!\n");
		other = yield(other);
	}
}

void thread2(void *other) {
	while(1) {
		printf("Thread 2 !!!\n");
		break;
		other = yield(other);
	}
}

void arch_main(MultiBootInfo *mb_info) {
	void *other_thread;
	SerialPort com1;
	FILE console;
	MuxWriter mux_writer;
	CPUInfo cpu_info;
	char cpu_vendor[13];
	uint32_t apic_id;
	int i;

	uintptr_t alignptr;

	gdt_init();
	serial_init(COM1, &com1);
	text_console_init(&console);
	muxwriter_init(&mux_writer, (FILE*)&com1, &console);
	stdout = (FILE*)&mux_writer;
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

	alignptr = (uintptr_t)kalloc_align(37, 0x100);
	printf("alignptr : 0x%x (should end in 00)\n", alignptr);

	memset(&cpu_info, 0, sizeof(CPUInfo));
	cpuid(&cpu_info);
	memcpy(&cpu_vendor[0], &cpu_info.ebx, sizeof(uint32_t));
	memcpy(&cpu_vendor[4], &cpu_info.edx, sizeof(uint32_t));
	memcpy(&cpu_vendor[8], &cpu_info.ecx, sizeof(uint32_t));
	cpu_vendor[12] = '\0';
	printf("Cpu: %s\n", cpu_vendor);
	
	if(!have_apic())
		panic("No apic found!");
	apic_id = get_apic_id();
	enable_apic();
	printf("Apic ID #%d is online.\n", apic_id);

	paging_init(mb_info->mem_upper * KIBI);
	other_thread = mk_thread(4 * KIBI, thread2);
	if(other_thread) {
		thread1(other_thread);
	} else {
		printf("kalloc_failed in %s near line %d!\n", __FILE__, __LINE__);
	}

	while(1);
}
