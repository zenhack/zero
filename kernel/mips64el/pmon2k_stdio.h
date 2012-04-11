#ifndef KERNEL_MIPS64EL_PMON2K_STDIO_H
#define KERNEL_MIPS64EL_PMON2K_STDIO_H

#include <kernel/mips64el/pmon2k.h>
#include <kernel/port/stdio.h>

uintptr_t pmon_write_string(FILE *out, void *buf, uintptr_t len);

void init_pmon_stdout(FILE *out, callvectors *cv);

#endif

