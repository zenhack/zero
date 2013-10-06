.globl _start
_start:
	// the beginning of the stack is too far away to load from here; move
	// closer to it.
	b init$

boot_stack_end$:
	.space 4096
boot_stack$:
init$:
	ldr sp, boot_stack$
	mov fp, sp
	bl arch_main
hang$:
	b hang$
