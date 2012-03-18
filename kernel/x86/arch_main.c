#include <kernel/port/stdio.h>
#include <kernel/x86/serial.h>

void arch_main(void) {
	FILE com1;
	serial_init(COM1, &com1);
	stdout = &com1;
	printf("Hello, World!\n");
	while(1);
}
