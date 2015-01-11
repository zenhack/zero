#include <stdint.h>
#include <kernel/rpi/act_led.h>
#include <kernel/rpi/gpio.h>
#include <kernel/port/mmio.h>

/* The GPIO pin assigned to the led: */
#define LED_PIN 16

/* Each of these works as follows:
 *
 * 1. set the pin as an output
 * 2. set or clear the pin, as appropriate.
 *
 * Note that these are *not* backwards -- clearing the pin turns the led *on*.
 */

void act_led_on(void) {
	gpio_fsel(LED_PIN, GPIO_F_OUT);
	gpio_clr(LED_PIN);
}

void act_led_off(void) {
	gpio_fsel(LED_PIN, GPIO_F_OUT);
	gpio_set(LED_PIN);
}
