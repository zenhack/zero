#include <kernel/port/string.h>

uintptr_t strlen(char *s) {
	uintptr_t size = 0;
	while (*s) {
		size++;
		s++;
	}
	return size;
}
