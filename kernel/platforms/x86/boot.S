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
	/* First, clear interrupts. */
	cli
	/* Then, load the GDT: */
	/* TODO: This is copypasta from gdt_load.S. We should factor this out. */
	movl $gdt_desc, %eax
	lgdt (%eax)
	movw $SEGOFF(KDATA_SEGMENT), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	/* Next, set the PE bit in cr0, which enables protected mode: */
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0

/* Both [intel/3/9.10] and [osdev/Protected_Mode] claim that we need to
 * execute a *far* jump here, in order to actually start executing protected
 * mode code. However,
 *
 * 1. The example code in [intel/3/9.10.2] doesn't do this, it does a *near*
 *    jump (with a comment that says "clear prefetch queue").
 * 2. Imperically, doing a far jump here seems to cause a triple fault.
 * 3. According to bochs after the near jump we're in protected mode.
 *
 * I'm not really satisifed with this; we ought to understand why this code
 * works, and I worry that once we implement a bit more we'll find
 * it actually doesn't. For now though, we follow the example in
 * [intel/3/9.10.2]. */
	jmp ap_protected_mode
ap_protected_mode:

	/* Now get yourself a stack: */
	movl LOCAL_APIC_ID, %eax
	addl 1, %eax
	/* Multiply by 4096; this gets us a memory offeset for the stack: */
	shll $12, %eax
	/* Add the base: */
	movl num_cpus, %edx
	addl %edx, %eax

	movl %eax, %esp
	movl 0, %ebp

/* Uncommenting this call results in a triple fault; we need to figure out why. */
//	call ap_main

hang_ap:
	jmp hang_ap

.bss
/* reserve space for a stack for the BSP. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:
