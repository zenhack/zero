#include <kernel/x86/8259pic.h>
#include <kernel/x86/portio.h>

/* resources: [8259], [osdev/PIC] */

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xa0
#define PIC_CMD(pic) (pic)
#define PIC_DATA(pic) ((pic)+1)

#define PIC_INIT 0x10
#define PIC_EOI 0x20

void disable_8259pic(void) {
	/* Even though we're disabling this in favor of the apic, it's
	 * apparently still possible for the 8259pic to generate spurrious
	 * interupts, so we should remap these to things that don't conflict
	 * with the CPU's exceptions. */
	out8(PIC_CMD(PIC_MASTER), PIC_INIT);
	out8(PIC_CMD(PIC_SLAVE), PIC_INIT);
	out8(PIC_DATA(PIC_MASTER), 0x20); /* last cpu exception is at 0x1f */
	out8(PIC_DATA(PIC_SLAVE), 0x28); /* right after the master pic */

	/* Tell the pics about their relationship. The slave is on the master's
	 * pin #2. */
	out8(PIC_DATA(PIC_MASTER), 1<<2); /* The master expects a bitmask of slaves */
	out8(PIC_DATA(PIC_SLAVE), 2); /* The slave expects the master irq number it's
					 attached to (a 3 bit field). */

	/* Okay, now actually disable the pic: */
	out8(PIC_DATA(PIC_MASTER), 0xff);
	out8(PIC_DATA(PIC_SLAVE), 0xff);
}
