#include <kernel/x86/gdt.h>
#include <kernel/x86/apic.h>

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

	/* int_handler_common will have returned a pointer to a possibly
	 * different set of interrupted state. We should restore to *that*
	 * state. Point the stack at it: */
	movl %eax, %esp

	/* clean up the stack and restore the old data segment. */
	popl %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	popa
	addl $8, %esp

	/* Signal the end of interrupt. We have to write to the APIC's EOI
         * register to do this, but it doesn't matter *what* we write, so we
         * just pick something that's handy.
         *
	 * Note that we need to insert some conditional logic here to avoid this
	 * for interrupts that shouldn't have it. These include "those delivered
	 * with the NMI, SMI, INIT, ExtINT, the start-up, or INIT-Deassert
	 * delivery mode" [intel/3/10.8.5] */ 
	movl %eax, EOI
	iret
