.globl get32
get32:
	ldr r0, [r0]
	mov pc, lr

.globl put32
put32:
	str r1, [r0]
	mov pc, lr
