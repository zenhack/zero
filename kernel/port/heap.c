#include <stddef.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/stdio.h>
#include <kernel/port/panic.h>

/* round_up returns `n` rounded to the next value that is zero
 * modulo `align`. */
static uintptr_t round_up(uintptr_t n, uintptr_t align) {
	if (n % align) {
		return n + (align - (n % align));
	}
	return n;
}

uintptr_t heap_next, heap_end;

void heap_init(uintptr_t start, uintptr_t end) {
	heap_next = start;
	heap_end = end;
}

void *kalloc_align(uintptr_t size, uintptr_t alignment) {
	heap_next = round_up(heap_next, alignment);
	uintptr_t ret = heap_next;
	heap_next += size;
	if(heap_next > heap_end) {
		panic("Out of space!");
	}
	return (void*)ret;
}

void *kalloc(uintptr_t size) {
	return kalloc_align(size, sizeof(uintptr_t));
}

void kfree(void *ptr, uintptr_t size) {
}
