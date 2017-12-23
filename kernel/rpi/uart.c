#include <kernel/rpi/uart.h>
#include <kernel/port/bitwhack.h>
#include <kernel/port/units.h>
#include <kernel/port/mmio.h>
#include <kernel/rpi/mmio.h>
#include <kernel/rpi/gpio.h>

#include <stdint.h>

#define UART_ENABLE (1<<0)

/* [bcm2835] says only bit 0 needs to be set for 8 bit mode, but [dwelch-uart]
 * claims this is a mistake - bit 1 needs to be set as well. */
#define UART_8BIT ((1<<0) | (1<<1))

#define IIR_CLEAR_RECV (1<<1)
#define IIR_CLEAR_SEND (1<<2)

#define TRANSMIT_PIN 14
#define RECEIVE_PIN 15

/* Set the baud rate. From [bcm2835] (sec 2.2.1), */

/** Return the baudrate register value necessary to set the buaud rate to
 * `baudrate`. */
static uint32_t baud_reg_from_rate(uint32_t baudrate) {
	/* From [bcm2835] (sec 2.2.1),
	 *
	 *     baudrate = system_clock_freq / (8 * (baudrate_reg + 1))
	 */

	/* The system clock for the rpi is: */
	uint32_t system_clock_freq = 700 * MEGA; /* XXX: Is this true? [dwelch-uart] seems to think it's 250 MHz. */
	/* So, the register value can be computed as: */
	return system_clock_freq / (8 * baudrate) - 1;
}

void uart_init(void) {
	gpio_fsel(TRANSMIT_PIN, GPIO_F_ALT0);
	gpio_fsel(RECEIVE_PIN,  GPIO_F_ALT0);

	/* Enable the UART. */
	put32(AUX_ENABLES, KEEP_LO(3) & (get32(AUX_ENABLES) | UART_ENABLE));

	/* Clear the DLAB, and set the data size to 8 bits. See also the
	 * definition of UART_8BIT - there's an error re: this in the spec. */
	put32(AUX_MU_LCR_REG, (KEEP_LO(8) | KEEP_RANGE(2, 5)) & UART_8BIT);


	/* Clear out the FIFOs: */
	put32(AUX_MU_IIR_REG, IIR_CLEAR_RECV | IIR_CLEAR_SEND);

	/* Set the baud rate to 115200 Hz: */
	put32(AUX_MU_BAUD_REG, KEEP_LO(16) | baud_reg_from_rate(115200));

}

#define GPIO_B 0x00200000
#define UART0_BASE (GPIO_B + 0x1000)
#define UART0_DR UART0_BASE
#define UART0_FR (UART0_BASE + 0x18)

void uart_putc(uint8_t c) {
#if 0
	while(!(get32(AUX_MU_STAT_REG) & (1<<9))); /* transmitter done */
	put32(AUX_MU_IO_REG, KEEP_LO(8) & c);
#endif
	while(get32(UART0_FR) & (1 << 5));
	put32(UART0_DR, c);
}
