#include <kernel/yeeloong/pmon2k_stdio.h>

uintptr_t pmon_write_string(FILE *out, void *buf, uintptr_t len) {
	uintptr_t written = 0;
	char *s = (char*)buf;
	callvectors *cv = (callvectors*)out->aux;
	while(len - written) {
		cv->printf("%c", *s);
		written++;
		s++;
	}
	return len;
}

void init_pmon_stdout(FILE *out, callvectors *cv) {
	out->aux = cv;
	out->write = pmon_write_string;
}
