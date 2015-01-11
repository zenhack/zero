#include <kernel/rpi/gpio.h>
#include <kernel/port/mmio.h>

void gpio_fsel(int pin, uint32_t func) {
	/* each of the GPFSELn registers contains 10 3-bit fields (30 bits + 2
	 * wasted), with each pin having its own field. we compute which
	 * register has our field, and what offset: */
	int reg = pin % 10;
	int off = (pin / 10) * 3;

	uint32_t gpfsel = get32(GPFSEL(reg));

	/* clear out the three bits for our pin, and give them the new value: */
	gpfsel &= ~(7<<off);
	gpfsel |= (func<<off);

	put32(GPFSEL(reg), gpfsel);
}

/* TODO: factor these two into a common helper. */
void gpio_set(int pin) {
	int reg = pin / 32;
	int off = pin % 32;
	put32(GPSET(reg), 1<<off);
}

void gpio_clr(int pin) {
	int reg = pin / 32;
	int off = pin % 32;
	put32(GPCLR(reg), 1<<off);
}
