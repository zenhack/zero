#include <stdint.h>
#include <kernel/rpi/sleep.h>
#include <kernel/rpi/morse.h>
#include <kernel/rpi/morse_led.h>

void arch_main(void) {
	init_morse_led(stdout);
	for(;;) {
		printf("hello world");
		sleep(10 * MORSE_UNIT);
	}
}
