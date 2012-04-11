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

int strncmp(char *s1, char *s2, size_t len) {
	while(*s1 == *s2 && len > 0) {
		s1++;
		s2++;
		len--;
	}
	if(*s1 == *s2)
		return 0;
	else if(*s1 > *s2)
		return 1;
	else
		return -1;
}

int strcmp(char *s1, char *s2) {
	return strncmp(s1, s2, (size_t)~0);
}

char *strchr(char *s, int c) {
	while(*s) {
		if(*s == c)
			return s;
		s++;
	}
	return NULL;
}
