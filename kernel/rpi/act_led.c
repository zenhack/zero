#include <stdint.h>
#include <kernel/rpi/act_led.h>
#include <kernel/rpi/mmio.h>

void act_led_on(void) {
	// enable output to the led:
	put32(GPFSEL1, 1<<18);
	// turn the led on:
	put32(GPCLR0, 1<<16);
}

void act_led_off(void) {
	put32(GPFSEL1, 1<<18);
	put32(GPSET0, 1<<16);
}
