#ifndef KERNEL_X86_APIC_TIMER_SETUP_H
#define KERNEL_X86_APIC_TIMER_SETUP_H

#include <stdint.h>

#define APIC_TIMER_INT_NO 255

/* Initialize the apic timer with the given frequency.
 *
 * Before this function is called, the apic_setup() must have exectued.
 *
 * This also properly remaps and disables the 8259pic (TODO (isd):
 * this is  conceptually orthoginal; should probably be done elsewhere,
 * though how exactly to organize that is something I need to think about).
 */
void apic_timer_setup(uint32_t frequency);

/* stores the frequency of the configured apic timer. This is set as a side
 * effect of calling apic_timer_setup(). */
extern uint32_t apic_timer_frequency;

/* stores the intial count for the apic used to achieve `apic_timer_frequency`. */
extern uint32_t apic_timer_init_count;

#endif
