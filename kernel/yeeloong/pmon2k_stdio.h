#ifndef KERNEL_YEELOONG_PMON2K_STDIO_H
#define KERNEL_YEELOONG_PMON2K_STDIO_H

#include <kernel/yeeloong/pmon2k.h>
#include <kernel/port/stdio.h>

typedef struct PMONFILE PMONFILE;
struct PMONFILE {
	FILE file;
	callvectors *cv;
};

uintptr_t pmon_write_string(FILE *out, void *buf, uintptr_t len);

void init_pmon_stdout(PMONFILE *out, callvectors *cv);

#endif

