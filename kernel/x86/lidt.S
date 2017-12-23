
/* simple wrapper around the lidt instruction. */
.globl lidt
lidt:
	movl 4(%esp), %eax
	lidt (%eax)
	ret
