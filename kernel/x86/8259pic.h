#ifndef KERNEL_X86_8259PIC_H
#define KERNEL_X86_8259PIC_H

/* Interrupt numbers for the IRQs. These start at 0x20, since the last
 * CPU exception is 0x1f. */
#define IRQ(N) ((N)+0x20)

/* Remap the PIC to avoid collisions with the CPU exceptions.
 * See [osdev/PIC]. */
void remap_8259pic(void);

/* Turn off the PIC. Must call remap first. */
void disable_8259pic(void);

#endif
