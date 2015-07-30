#include <kernel/port/panic.h>
#include <kernel/port/stdio.h>
#include <kernel/x86/asm.h>

void panic(char *s) {
	printf("KERNEL PANIC: %s\n", s);
	while(1) hlt();
}
