#include <kernel/yeeloong/pmon2k_stdio.h>

uintptr_t pmon_write_string(FILE *out, void *buf, uintptr_t len) {
	uintptr_t written = 0;
	char *s = (char*)buf;
	callvectors *cv = ((PMONFILE*)out)->cv;
	while(len - written) {
		cv->printf("%c", *s);
		written++;
		s++;
	}
	return len;
}

void init_pmon_stdout(PMONFILE *out, callvectors *cv) {
	out->cv = cv;
	out->file.write = pmon_write_string;
}
