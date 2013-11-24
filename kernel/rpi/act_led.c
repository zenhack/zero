#include <stdint.h>
#include "act_led.h"
#include "mmio.h"

#define GPIO_BASE (IO_BASE + 0x00200000)

#define GPFSEL1 (GPIO_BASE + 4)
#define GPSET0 (GPIO_BASE + 28)
#define GPCLR0 (GPIO_BASE + 40)

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
