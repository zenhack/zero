.globl sleep
sleep:
	subs r0, r0, #1
	bne sleep
	mov pc, lr
