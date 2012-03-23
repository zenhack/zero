#ifndef KERNEL_X86_ISR_H
#define KERNEL_X86_ISR_H

#include "idt.h"

extern void (*isrs[256])();

#endif
