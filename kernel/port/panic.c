#include <kernel/port/stdio.h>

void panic(char *s)
{
	printf("KERNEL PANIC: %s\n", s);
	while(1);
}
