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
	/* First, load the GDT: */
	movl $gdt_desc, %eax
	lgdt (%eax)
	movw $SEGOFF(KDATA_SEGMENT), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	jmpl $SEGOFF(KCODE_SEGMENT), $ap_code_segment
ap_code_segment:
	/* Next, get yourself a stack: */
	movl LOCAL_APIC_ID, %eax
	addl 1, %eax
	/* Multiply by 4096; this gets us a memory offeset for the stack: */
	shll $12, %eax

	/* Add the base: */
	movl num_cpus, %edx
	addl %edx, %eax

	movl %eax, %esp
	movl 0, %ebp
	cli
	call ap_main

	hlt
	jmp hang

.bss
/* reserve space for a stack for the BSP. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:
