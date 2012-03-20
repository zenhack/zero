#include <kernel/x86/gdt.h>

.globl gdt_load
gdt_load:
	movl 4(%esp), %eax
	lgdt (%eax)
	movw $SEGOFF(KDATA_SEGMENT), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	jmpl $SEGOFF(KCODE_SEGMENT), $code_segment
code_segment:
	ret
