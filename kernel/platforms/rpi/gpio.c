#include <kernel/rpi/gpio.h>
#include <kernel/port/mmio.h>

#define GPIO_BASE (IO_BASE + 0x00200000)
#define GPIO_INDEX(off, idx) (GPIO_BASE + (off) + ((idx)<<2))

#define GPFSEL 0x0
#define GPSET 0x1c
#define GPCLR 0x28

#define GPPUD (GPIO_BASE + 0x94)
#define GPPUDCLK(n) GPIO_INDEX(0x98, n)

void gpio_fsel(int pin, uint32_t func) {
	/* each of the GPFSELn registers contains 10 3-bit fields (30 bits + 2
	 * wasted), with each pin having its own field. we compute which
	 * register has our field, and what offset: */
	int reg = pin % 10;
	int off = (pin / 10) * 3;

	uint32_t gpfsel = get32(GPIO_INDEX(GPFSEL, reg));

	/* clear out the three bits for our pin, and give them the new value: */
	gpfsel &= ~(7<<off);
	gpfsel |= (func<<off);

	put32(GPIO_INDEX(GPFSEL, reg), gpfsel);
}


/* helper for gpio_set & gpio_clear */
static void clr_or_set(int pin, int bank) {
	int reg = pin / 32;
	int off = pin % 32;
	put32(GPIO_INDEX(bank, reg), 1<<off);
}

void gpio_set(int pin) { clr_or_set(pin, GPSET); }
void gpio_clr(int pin) { clr_or_set(pin, GPCLR); }
