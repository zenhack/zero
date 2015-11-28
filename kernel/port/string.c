#include <kernel/port/string.h>

size_t strlen(char *s) {
	size_t size = 0;
	while (*s) {
		size++;
		s++;
	}
	return size;
}

int memcmp(const void *s1, const void *s2, size_t len) {
	const uint8_t *a, *b;
	a = s1;
	b = s2;

	while(len > 0) {
		if(*a > *b) {
			return 1;
		} else if(*a < *b) {
			return -1;
		} else {
			a++;
			b++;
			len--;
		}
	}
	return 0;
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

int strncmp(const char *s1, const char *s2, size_t len) {
	while(len > 0 && *s1 && *s2) {
		if(*s1 > *s2) {
			return 1;
		} else if (*s1 < *s2) {
			return -1;
		} else {
			s1++;
			s2++;
			len--;
		}
	}
	return 0;
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
