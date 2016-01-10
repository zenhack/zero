#include <kernel/x86/multiboot.h>

#define MULTIBOOT_FLAGS MULTIBOOT_MEMINFO

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
	hlt
	jmp ap_start

.bss
/* reserve space for a stack. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:
