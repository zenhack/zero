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
#include <kernel/x86/hlt.h>

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void arch_main(MultiBootInfo *mb_info) {
	SerialPort com1;
	FILE console;
	MuxWriter mux_writer;
	uint32_t apic_id;

	gdt_init();

	/* direct output to both the serial line and video console. */
	serial_init(COM1, &com1);
	text_console_init(&console);
	muxwriter_init(&mux_writer, (FILE*)&com1, &console);
	stdout = (FILE*)&mux_writer;

	idt_init();

	printf("Booting Zero...\n");
	printf("\n");
	printf("Mutliboot info struct located at 0x%x\n", mb_info);
	printf("  High memory : 0x%x\n", mb_info->mem_upper * KIBI);
	printf("  Kernel image ends at : 0x%x\n", &kend);

	/* initialize the heap, using the chunk of memory between the
	 * kernel and mem_upper. */
	heap_init((uintptr_t)&kend, (uintptr_t)mb_info->mem_upper * KIBI);
	
	if(!have_apic()) {
		panic("No apic found!\n");
	}
	apic_id = get_apic_id();
	enable_apic();
	printf("Apic ID #%d is online.\n", apic_id);

	paging_init(mb_info->mem_upper * KIBI);

	while(1) hlt();
}
