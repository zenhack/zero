#include <kernel/rpi/morse_led.h>
#include <kernel/rpi/morse.h>

static size_t morse_led_write(FILE *out, void *buf, size_t len) {
	char *str = (char *)buf;
	morse_write(str, len);
	return len;
}

void init_morse_led(FILE *out) {
	out->write = morse_led_write;
}
