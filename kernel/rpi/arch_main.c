#include <stdint.h>
#include <kernel/rpi/sleep.h>
#include <kernel/rpi/morse.h>
#include <kernel/rpi/morse_led.h>

#include <kernel/rpi/uart.h>

static size_t _raspbootin_uart_write(FILE *out, void *buf, size_t len) {
	void (*_putc)(char) = (void (*)(char))0x02000190;
	uint8_t *bytes = buf;
	for(size_t i = 0; i < len; i++) {
		_putc(bytes[i]);
	}
	return len;
}

char *s = "Hello, World!\n";

void arch_main(void) {
#if 0
//	int i;

//	uart_init();
	for(int i = 0; i < 10; i++) {
#if 0
		for(char *_s = s; *_s; _s++) {
			_putc(*_s);
		}
#endif
		for(char *_s = s; *_s; _s++) {
			uart_putc(*_s);
		}
	}
	while(1);
#if 0

//	init_morse_led(stdout);
//	uart_init();
	_putc('A');
	for(i = 0; i < 10; i++) {
		_putc('A' + i);
	}
	for(;;) {
//		printf("hello world");
//		sleep(10 * MORSE_UNIT);
	}
#endif
#endif
	stdout->write = _raspbootin_uart_write;
	printf("Hello, World!\n");
	while(1);
}
