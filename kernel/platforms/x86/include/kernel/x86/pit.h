#ifndef KERNEL_X86_PIT_H
#define KERNEL_X86_PIT_H

#include <stdint.h>

/* Initialize the PIT with the given frequencey, in Hz.
 *
 * Before this function is invoked there MUST be an interrupt handler registered
 * for IRQ 0.
 */
void pit_init(uint32_t frequency);

#endif
