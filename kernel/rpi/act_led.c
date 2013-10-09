#include <stdint.h>
#include "act_led.h"

#define IO_BASE 0x20000000
#define GPIO_BASE (IO_BASE + 0x00200000)

void act_led_on(void) {
	// enable output to the led:
	*((uint32_t*)(GPIO_BASE + 4)) = (1<<18);
	// turn the led on:
	*((uint32_t*)(GPIO_BASE + 40)) = (1<<16);
}

void act_led_off(void) {
	*((uint32_t*)(GPIO_BASE + 4)) = (1<<18);
	*((uint32_t*)(GPIO_BASE + 28)) = (1<<16);
}
