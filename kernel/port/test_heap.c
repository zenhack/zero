
#include <assert.h>
/* XXX */
#define KERNEL_PORT_STDIO_H

#include <kernel/port/heap.c>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VERIFY(expr) \
	do { \
		fprintf(stderr, "Checking: %s\n", #expr); \
		assert(expr); \
	} while(0)


void panic(char *s) {
	fprintf(stderr, "KERNEL PANIC: %s\n", s);
	exit(1);
}

void make_heap(void) {
	uintptr_t size = 20 * MEBI;
	uintptr_t start = (uintptr_t)malloc(size);
	uintptr_t end = start + size;
	heap_init(start, end);
}

int main(void) {
	VERIFY(round_up(4, 4) == 4);
	VERIFY(round_down(4, 4) == 4);
	VERIFY(round_up(5, 4) == 8);
	VERIFY(round_down(5, 4) == 4);

	make_heap();

	void *a = kalloc(10);
	void *b = kalloc(10);
	VERIFY(a != b);

	a = kalloc_align(8192, 4096);
	b = kalloc_align(8192, 4096);

	VERIFY(a != b);
}
