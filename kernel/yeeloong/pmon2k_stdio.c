#include <kernel/yeeloong/pmon2k_stdio.h>

size_t pmon_write_string(FILE *out, void *buf, size_t len) {
	size_t written = 0;
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
