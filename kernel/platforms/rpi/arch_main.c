#include <stdint.h>
#include <kernel/rpi/sleep.h>
#include <kernel/rpi/morse.h>
#include <kernel/rpi/morse_led.h>

#include <kernel/rpi/uart.h>

void arch_main(void) {
	int i;

	init_morse_led(stdout);
	uart_init();
	uart_putc('A');
	for(i = 0; i < 10; i++) {
		uart_putc('A');
	}
	for(;;) {
		printf("hello world");
		sleep(10 * MORSE_UNIT);
	}
}
