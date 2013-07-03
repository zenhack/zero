#ifndef KERNEL_X86_TEXT_CONSOLE_H
#define KERNEL_X86_TEXT_CONSOLE_H

#include <stdint.h>
#include <kernel/port/stdio.h>

void text_console_init(FILE *stream);

#endif
