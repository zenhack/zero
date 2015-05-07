#include <kernel/x86/pit.h>
#include <kernel/x86/portio.h>

#define PIT_DATA0_PORT 0x40
/* The PIT has two other data ports, but we aren't interested. */
#define PIT_CMD_PORT 0x43

#define INPUT_FREQUENCY 1193180

#define PIT_RATE_GEN_MODE (2<<1)
#define PIT_ACCESS_LO (1<<4)
#define PIT_ACCESS_HI (1<<5)

void pit_init(uint32_t frequency) {
	uint32_t divisor = INPUT_FREQUENCY / frequency;
	out8(PIT_CMD_PORT, PIT_RATE_GEN_MODE|PIT_ACCESS_LO|PIT_ACCESS_HI);
	out8(PIT_DATA0_PORT, divisor & 0xff);
	out8(PIT_DATA0_PORT, (divisor>>8) & 0xff);
}
