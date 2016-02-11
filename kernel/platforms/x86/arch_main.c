#include <kernel/port/heap.h> /* heap_init */
#include <kernel/port/stdio.h> /* printf */
#include <kernel/port/units.h> /* KIBI */
#include <kernel/x86/apic_setup.h> /* apic_setup */
#include <kernel/x86/apic_timer_setup.h> /* apic_timer_setup */
#include <kernel/x86/asm.h> /* hlt */
#include <kernel/x86/gdt.h> /* gdt_load, gdt_desc */
#include <kernel/x86/idt.h> /* idt_init */
#include <kernel/x86/mp_setup.h> /* mp_setup */
#include <kernel/x86/multiboot.h> /* MultiBootInfo */
#include <kernel/x86/paging.h> /* paging_init */
#include <kernel/x86/stdio_setup.h> /* stdio_init */
#include <kernel/x86/common_main.h> /* common_main */


/* defined in link.ld; located at the end of the kernel image. */
extern void *kend;

MultiBootInfo my_mb_info;

void arch_main(MultiBootInfo *mb_info) {

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

	/* Initialize the heap. We use the first contiguous region in "high
	 * memory" (only accessible from protected mode). We'll of course want
	 * to change this at some point. */
	heap_init(1 * MEBI, (uintptr_t)mb_info->mem_upper * KIBI);

	paging_init(mb_info->mem_upper * KIBI);

	apic_setup();
	apic_timer_setup(1024);

	mp_setup();

	common_main();
}
