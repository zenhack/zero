#include <stdint.h>
#include "act_led.h"
#include "mmio.h"

#define GPIO_BASE (IO_BASE + 0x00200000)

#define GPFSEL1 (GPIO_BASE + 4)
#define GPSET0 (GPIO_BASE + 28)
#define GPCLR0 (GPIO_BASE + 40)

void act_led_on(void) {
	// enable output to the led:
	*((uint32_t*)GPFSEL1) = (1<<18);
	// turn the led on:
	*((uint32_t*)GPCLR0) = (1<<16);
}

void act_led_off(void) {
	*((uint32_t*)GPFSEL1) = (1<<18);
	*((uint32_t*)GPSET0) = (1<<16);
}
