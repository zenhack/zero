#ifndef KERNEL_PORT_PANIC_H
#define KERNEL_PORT_PANIC_H

/* Makes the kernel panic, outputs string s as an error message. */
void panic(char *s);
#endif
