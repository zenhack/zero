#include <kernel/x86/gdt.h>

/* This stub is called from the interrupt handlers in isr.c */

.globl isr_stub
isr_stub:
	pusha

	/* save the old data segment */
	movw %ds, %ax
	pushl %eax
	
	/* load the kernel data segment: */
	movw $SEGOFF(KDATA_SEGMENT), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	/* we want to pass a pointer to all this to the C function,
	   otherwise optimizations might cause it to be overwritten */
	movl %esp, %eax
	pushl %eax

	cld
	call int_handler_common

	/* clean up the stack and restore the old data segment. */
	popl %eax
	popl %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	popa
	addl $8, %esp

	iret
