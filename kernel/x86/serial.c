#include <kernel/x86/serial.h>
#include <kernel/x86/portio.h>

/* offsets from serial port address: */
static const uint16_t
/* when DLAB = 0 : */
	DATA_REG = 0,
	INT_ENABLE =1,
/* when DLAB = 1 : */
	BAUD_DIV_LSB = 0,
	BAUD_DIV_MSB = 1,
/* regardless of DLAB: */
	INT_ID_FIFO = 2,
	LINE_CNTL_REG = 3, /* DLAB is the MSB */
	MODEM_CNTL_REG = 4,
	LINE_STATUS_REG = 5,
	MODEM_STATUS_REG = 6,
	SCRATCH_REG = 7;

static const uint8_t
	DLAB_SET = (1<<7),
	DLAB_CLEAR = 0,
	DATABITS_8 = 0x3,
	NO_PARITY = 0,
	LINE_READY_STATUS = (1<<5);

static size_t serial_write(FILE *stream, void *buf, size_t len);

void serial_init(uint16_t port, SerialPort *stream) {
	out8(port+INT_ENABLE, 0); /* disable interrupts on the device. */
	out8(port+LINE_CNTL_REG, DLAB_SET); /* enable DLAB */
	/* set baud rate divisor: */
	out8(port+BAUD_DIV_LSB, 0x3);
	out8(port+BAUD_DIV_MSB, 0x0);
	/* clear DLAB, ask for 8 bit chars with no pairty: */
	out8(port+LINE_CNTL_REG, DLAB_CLEAR | DATABITS_8 | NO_PARITY);

	/* set up the stream */
	stream->file.write = serial_write;
	stream->port = port;
}

static void serial_putc(uint16_t port, uint8_t data) {
	while(!(in8(port+LINE_STATUS_REG) & LINE_READY_STATUS));
	out8(port, data);
}

static size_t serial_write(FILE *stream, void *buf, size_t len) {
	uint8_t *bytes = (uint8_t*)buf;
	size_t left = len;
	uint16_t port = ((SerialPort*)stream)->port;

	while(left) {
		serial_putc(port, *bytes);
		bytes++;
		left--;
	}
	return len;
}
