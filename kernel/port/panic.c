#include <kernel/port/stdio.h>

__attribute__((weak))
void panic(char *s)
{
	printf("KERNEL PANIC: %s\n", s);
	while(1);
};
