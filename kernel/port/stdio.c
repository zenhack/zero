#include <kernel/port/stdio.h>
#include <kernel/port/string.h>
#include <stdarg.h>

FILE *stdout;

static void fprintint(FILE *stream, int num, int base);
static void fprints(FILE *stream, char *s);
static int fprintf_arg(FILE *stream, char *fmt, va_list ap);

int fprintf(FILE *stream, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	return fprintf_arg(stream, fmt, ap);
}

int printf(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	return fprintf_arg(stdout, fmt, ap);
}

int fprintf_arg(FILE *stream, char *fmt, va_list ap) {
	char c;

	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			switch(*fmt) {
			case '%':
				stream->write(stream, fmt, sizeof(char));
				break;
			case 'x':
				fprintint(stream, va_arg(ap, int), 16);
				break;
			case 'd':
				fprintint(stream, va_arg(ap, int), 10);
				break;
			case 'o':
				fprintint(stream, va_arg(ap, int), 8);
				break;
			case 'b':
				fprintint(stream, va_arg(ap, int), 2);
				break;
			case 's':
				fprints(stream, va_arg(ap, char *));
				break;
			case 'c':
				c = (char)va_arg(ap, int);
				stream->write(stream, &c, sizeof(char));
				break;
			default:
				fprintf(stream, "<< INVALID FORMAT CHAR: 0x%x >>", *fmt);
				return -1;
			}
		} else {
			stream->write(stream, fmt, sizeof(char));
		}
		fmt++;
	}
	return 0;
}

static void fprints(FILE *stream, char *s) {
	stream->write(stream, s, strlen(s));
}

static void fprintint(FILE *stream, int num, int base) {
	char s[8*sizeof(int)+1];

	int i = 8*sizeof(int);
	s[i--] = '\0';

	if(num == 0) {
		fprints(stream, "0");
		return;
	}
	while(num) {
		int digit = num % base;
		num /= base;
		s[i--] = "0123456789abcdef"[digit];
	}
	stream->write(stream, &s[i+1], strlen(&s[i+1]));
}
