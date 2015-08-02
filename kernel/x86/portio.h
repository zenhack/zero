#ifndef KERNEL_X86_PORTIO_H
#define KERNEL_X86_PORTIO_H

#include <stdint.h>

static inline void out8(uint16_t port, uint8_t data) {
	__asm__ volatile("outb %b[data], %w[port]"
		:: [data] "a" (data), [port] "Nd" (port));
}

static inline uint8_t in8(uint16_t port) {
	uint8_t ret;
	__asm__ volatile("inb %w[port], %b[ret]"
		: [ret] "=a" (ret)
		: [port] "Nd" (port));
	return ret;
}

#endif
