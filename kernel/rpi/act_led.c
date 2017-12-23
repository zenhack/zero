#include <stdint.h>
#include <kernel/rpi/act_led.h>
#include <kernel/rpi/gpio.h>
#include <kernel/port/mmio.h>

/* The GPIO pin assigned to the led. The RPI B+ and B2 supposedly use a
 * different pin that the original RPI. Neither one of these actually seems
 * to work on my B2 though. Pin 16 is for the older models, which is
 * currently enabled. Pin 47 is supposed to be the right value for newer
 * models. */
#define LED_PIN 16
/* #define LED_PIN 47 */

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
