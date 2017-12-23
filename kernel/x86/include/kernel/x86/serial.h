#ifndef KERNEL_X86_SERIAL_H
#define KERNEL_X86_SERIAL_H

#include <stdint.h>
#include <kernel/port/stdio.h>

static const uint16_t COM1 = 0x3f8;

typedef struct SerialPort SerialPort;
struct SerialPort {
	FILE file;
	uint16_t port;
};

/* sets up the serial port `port` and directs stream to write to it. */
void serial_init(uint16_t port, SerialPort *stream);

#endif
