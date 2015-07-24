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
#include <kernel/x86/paging.h>
#include <kernel/x86/hlt.h>
#include <kernel/x86/8259pic.h>
#include <kernel/x86/pit.h>
#include <kernel/x86/apic_timer.h>
#include <kernel/port/sched.h>
#include <kernel/x86/thread.h>

Regs *timer_interrupt(Regs *old_ctx) {
	Regs *new_ctx new_ctx = (Regs *)sched((void *)old_ctx);
//	send_8259pic_EOI(0);
	return new_ctx;
}

void example_thread(void *data) {
	char *msg = (char *)data;
	while(1) {
		printf("%s\n", msg);
	}
}

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void arch_main(MultiBootInfo *mb_info) {
	SerialPort com1;
	FILE console;
	MuxWriter mux_writer;
	uint32_t local_apic_id;
	MultiBootInfo my_mb_info;
	int i;

	/* We're going to start touching memory before too long, and we don't
	 * actually know where this struct is. let's get our own copy and use
	 * that: */
	my_mb_info = *mb_info;
	mb_info = &my_mb_info;

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

	remap_8259pic();
	for(i = 0; i < 16; i++) {
		register_int_handler(IRQ(i), ignore_8259pic_irq);
	}
	disable_8259pic();
	if(!have_apic()) {
		panic("No apic found!\n");
	}
	local_apic_id = get_local_apic_id();
	enable_local_apic();
	printf("Local Apic ID #%d is online.\n", local_apic_id);

	register_int_handler(255, timer_interrupt);
//	register_int_handler(IRQ(0), timer_interrupt);
	apic_timer_init(255, 7, APIC_TIMER_PERIODIC);
	apic_timer_set(200);

	X86Thread *threadA = mk_thread(8 * KIBI, example_thread, "A");
	X86Thread *threadB = mk_thread(8 * KIBI, example_thread, "B");

	sched_insert((Thread *)threadA);
	sched_insert((Thread *)threadB);

	paging_init(mb_info->mem_upper * KIBI);

//	pit_init(1024);

	asm volatile("sti");

	while(1) {
		printf("idle");
//		hlt();
	}
}
