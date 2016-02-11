#include <kernel/x86/multiboot.h>
#include <kernel/x86/apic.h>
#include <kernel/x86/gdt.h>

.extern num_cpus
.extern ap_stacks
.extern gdt_desc

#define MULTIBOOT_FLAGS MULTIBOOT_MEMINFO
#define AP_STACK_SIZE 4096

.globl multiboot
.align 4
multiboot:
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long MULTIBOOT_CHKSUM(MULTIBOOT_FLAGS)

/* Entry point for the bootstrap processor. This is invoked by grub. */
.globl bsp_start
bsp_start:
	/* load the stack */
	lea boot_stack-4, %esp
	mov 0, %ebp

	/* multiboot specifies that %ebx contains a pointer to the multiboot info
 	   structure. Pass it to arch_main. */
	pushl %ebx

	/* call our C routine to get us started. */
	cli
	call arch_main

	/* arch_main *shouldn't* return, but if it does, hang. */
hang:
	jmp hang

/* Entry point for the application processors (APs). These are started by an IPI
 * sent from the BSP. It is aligned on a page boudnary so the APs can be
 * directed to it. */
.align 4096
.globl ap_start
ap_start:
	.code16
	/** This is the standard logic for switching into protected mode
         * described by [osdev/Protected_Mode] and the intel manual. **/

	/* First, clear interrupts. */
	cli

	/* Then, load the GDT: */
	movl $gdt_desc, %eax
	lgdt (%eax)

	/* Next, set the PE bit in cr0, which enables protected mode: */
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0

	/* Now do a far jump to actually enter protected_mode: */
	jmpl $SEGOFF(KCODE_SEGMENT), $ap_protected_mode
ap_protected_mode:
	.code32

	/* The gdt pointer has already been loaded, but we need to make sure
	 * we're actually using its contents: */
	movw $SEGOFF(KDATA_SEGMENT), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	jmpl $SEGOFF(KCODE_SEGMENT), $with_new_gdt
with_new_gdt:


	/* set up a stack: */
	movl $ap1_stack-4, %esp
	movl $0, %ebp

	/* ...and call the C code: */
	call ap_main
ap_hang:
	jmp hang


#if 0
	/* We'll need this when we start dealing with > 1 AP; The code above
	 * just uses a stack at a fixed address, which only works for one cpu.
	 */

	/* Now get yourself a stack: */
	movl $LOCAL_APIC_ID, %edx
	movl (%edx), %eax
	addl 1, %eax
	/* Multiply by 4096; this gets us a memory offeset for the stack: */
	shll $12, %eax
	/* Add the base: */
	movl $num_cpus, %edx
	addl (%edx), %eax
#endif

.bss
/* reserve space for a stack for the BSP. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:
	.align 16
	.space 0x4000
ap1_stack:
