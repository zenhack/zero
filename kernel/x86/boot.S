#include <kernel/x86/multiboot.h>

.globl start
.globl multiboot

#define MULTIBOOT_FLAGS 0x0

.align 4
multiboot:
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long MULTIBOOT_CHKSUM(MULTIBOOT_FLAGS)

/* reserve space for a stack. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:

start:
	/* load the stack */
	lea boot_stack-4, %esp
	mov 0, %ebp

	/* call our C routine to get us started. */
	cli
	call arch_main

	/* arch_main *shouldn't* return, but if it does, hang. */
hang:
	jmp hang

