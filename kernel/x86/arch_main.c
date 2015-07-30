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
#include <kernel/x86/asm.h>
#include <kernel/x86/8259pic.h>
#include <kernel/x86/pit.h>
#include <kernel/x86/apic_timer.h>
#include <kernel/port/sched.h>
#include <kernel/x86/thread.h>


/* If we don't put volatile on these, the loop that checks if they've hit
 * appropriate values may be optimized to cache them in registers, which
 * obviously doesn't work. */
volatile uint32_t pit_ticks, apic_ticks;

static Regs *pit_calibrate_apic(Regs *old_ctx) {
	pit_ticks++;
	send_8259pic_EOI(0);
	return old_ctx;
}

static Regs *apic_calibrate_apic(Regs *old_ctx) {
	apic_ticks++;
	return old_ctx;
}

static Regs *apic_timer_sched(Regs *old_ctx) {
	return (Regs *)sched((void *)old_ctx);
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
	if(!have_apic()) {
		panic("No apic found!\n");
	}
	local_apic_id = get_local_apic_id();
	enable_local_apic();
	printf("Local Apic ID #%d is online.\n", local_apic_id);

	register_int_handler(255, apic_calibrate_apic);
	register_int_handler(IRQ(0), pit_calibrate_apic);
	apic_timer_init(255, 7, APIC_TIMER_PERIODIC);

	paging_init(mb_info->mem_upper * KIBI);

	printf("Measuring APIC timer frequency...\n");
	apic_timer_set(1024);
	pit_init(1024);

	sti();

	while(1) {
		hlt();
		if(pit_ticks >= 100) {
			cli();
			break;
		}
	}

	uint32_t new_init_count = (1024 * pit_ticks) / apic_ticks;

	printf(
		"  APIC ticks: %d\n"
		"  PIT ticks: %d\n"
		"  ratio: %d\n"
		"  new apic_start: %d\n",

		pit_ticks,
		apic_ticks,
		apic_ticks/pit_ticks,
		new_init_count
	);

	apic_timer_set(new_init_count);
	register_int_handler(IRQ(0), ignore_8259pic_irq);
	register_int_handler(255, apic_timer_sched);

	disable_8259pic();

	X86Thread *threadA = mk_thread(8 * KIBI, example_thread, "A");
	X86Thread *threadB = mk_thread(8 * KIBI, example_thread, "B");

	sched_insert((Thread *)threadA);
	sched_insert((Thread *)threadB);

	sti();

	while(1) {
		hlt();
	}
}
