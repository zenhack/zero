#include <kernel/x86/multiboot.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/stdio_setup.h>
#include <kernel/port/units.h>
#include <kernel/port/heap.h>
#include <kernel/port/string.h>
#include <kernel/port/panic.h>
#include <kernel/x86/gdt.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/cpuid.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/apic_setup.h>
#include <kernel/x86/paging.h>
#include <kernel/x86/asm.h>
#include <kernel/x86/8259pic.h>
#include <kernel/x86/pit.h>
#include <kernel/x86/apic_timer_setup.h>
#include <kernel/port/sched.h>
#include <kernel/x86/thread.h>

#include <kernel/port/mmio.h>


void example_thread(void *data) {
	char *msg = (char *)data;
	while(1) {
		printf("%s\n", msg);
	}
}

static Regs *test_show_local_apic_id(Regs *old_ctx) {
	printf("%d\n", get_local_apic_id());
	return old_ctx;
}

/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

void arch_main(MultiBootInfo *mb_info) {
	MultiBootInfo my_mb_info;

	/* We're going to start touching memory before too long, and we don't
	 * actually know where this struct is. let's get our own copy and use
	 * that: */
	my_mb_info = *mb_info;
	mb_info = &my_mb_info;

	gdt_load(&gdt_desc);
	idt_init();
	stdio_init();

	printf("Booting Zero...\n");
	printf("\n");
	printf("Mutliboot info struct located at 0x%x\n", mb_info);
	printf("  High memory : 0x%x\n", mb_info->mem_upper * KIBI);
	printf("  Kernel image ends at : 0x%x\n", &kend);

	/* initialize the heap, using the chunk of memory between the
	 * kernel and mem_upper. */
	heap_init((uintptr_t)&kend, (uintptr_t)mb_info->mem_upper * KIBI);

	paging_init(mb_info->mem_upper * KIBI);

	apic_setup();
	apic_timer_setup();

//	X86Thread *threadA = mk_thread(example_thread, "A");
//	X86Thread *threadB = mk_thread(example_thread, "B");

//	sched_insert((Thread *)threadA);
//	sched_insert((Thread *)threadB);

	/** test sending IPIs: */
	register_int_handler(254, test_show_local_apic_id);
	ApicICR icr;
	icr.lo.raw = get32(INT_COMMAND);

	icr.lo.dest_shorthand = ICR_IPI_ALL_BUT_SELF;
	icr.lo.deliv_mode = ICR_DELIV_INIT;
	icr.lo.level = ICR_ASSERT;
	put32(INT_COMMAND, icr.lo.raw);

	sti();

	while(1) {
		hlt();
	}
}
