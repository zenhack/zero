#ifndef KERNEL_X86_APIC_TIMER_H
#define KERNEL_X86_APIC_TIMER_H

#include <stdint.h>

#define APIC_TIMER_ONESHOT 0
#define APIC_TIMER_PERIODIC 1

/* Initializes and configures the local apic timer.
 *
 * - `int_no` is the interrupt number which the timer should trigger.
 * - `divisor` is a number to devide the bus clock by, which will set the
 *   frequency of the timer. This should be the exponent of a power of 2,
 *   with a maximum value of 7 (2^128).
 * - `mode` is the timer mode to use -- either APIC_TIMER_ONESHOT or
 *   APIC_TIMER_PERIODIC.
 */
void apic_timer_init(uint8_t int_no, uint8_t divisor, uint8_t mode);

/* Set the inital count register on the apic timer. The effect of this depends
 * on the timer's mode. */
void apic_timer_set(uint32_t value);

#endif
