#include <kernel/x86/apic.h>
#include <kernel/port/mmio.h>

void apic_timer_init(uint8_t int_no, uint8_t divisor, uint8_t mode) {

	/** set the divisor: **/

	uint32_t reg = get32(DIVIDE_CONF);
	reg &= ~0xf;
	/* The representation of the divisor in the divide configuration
	 * register is... weird. We're normalizing it a bit here; it's split up
	 * within the register, and for some reason 7 is divide by 1, where as
	 * the rest are a perfect 2^(n-1). See the intel manual for the
	 * details. */
	if (divisor == 0) divisor  = 7;
	else              divisor -= 1;
	reg |= (divisor & 0x3) | ((divisor & 0x4)<<1);
	put32(DIVIDE_CONF, reg);

	/** set the lvt entry: **/
	LVTEnt lvt_ent;
	lvt_ent.raw = get32(LVT_TIMER);
	lvt_ent.v.timer_mode = mode;
	lvt_ent.v.vector = int_no;
	lvt_ent.v.masked = 0;
	put32(LVT_TIMER, lvt_ent.raw);

}

void apic_timer_set(uint32_t value) {
	put32(INITIAL_COUNT, value);
}
