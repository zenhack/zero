#include <kernel/port/muxwriter.h>

static uintptr_t muxwriter_write(FILE *file, void *buf, uintptr_t len) {
	MuxWriter *mux = (MuxWriter *)file;
	mux->files[0]->write(mux->files[0], buf, len);
	return mux->files[1]->write(mux->files[1], buf, len);
}

void muxwriter_init(MuxWriter *mux, FILE *a, FILE *b) {
	mux->write = muxwriter_write;
	mux->files[0] = a;
	mux->files[1] = b;
}
