#include <stdint.h>
#include <kernel/rpi/morse.h>
#include <kernel/rpi/sleep.h>

void arch_main(void) {
	for(;;) {
		morse_string("hello world");
		sleep(10 * MORSE_UNIT);
	}
}
