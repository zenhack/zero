#include <kernel/port/string.h>

uintptr_t strlen(char *s) {
	uintptr_t size = 0;
	while (*s) {
		size++;
		s++;
	}
	return size;
}

void *memcpy(void *dest, const void *src, size_t len) {
	uint8_t *to, *from;
	to = (uint8_t*)dest;
	from = (uint8_t*)src;
	while(len) {
		*to++ = *from++;
		len--;
	}
	return dest;
}

void *memset(void *s, int c, size_t n) {
	uint8_t *data = (uint8_t*)s;
	while(n) {
		*data++ = c;
		n--;
	}
	return s;
}
