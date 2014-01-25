#include <kernel/x86/multiboot.h>

#define MULTIBOOT_FLAGS MULTIBOOT_MEMINFO

.globl multiboot
.align 4
multiboot:
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long MULTIBOOT_CHKSUM(MULTIBOOT_FLAGS)

.globl start
start:
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

.bss
/* reserve space for a stack. */
	.align 16
	.space 0x4000 /* relatively arbitrary */
boot_stack:
