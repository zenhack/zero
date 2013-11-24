.globl _start
_start:
	mov sp, #0x8000 // put the stack right below our entry point.
	mov fp, sp
	bl arch_main
hang$:
	b hang$
