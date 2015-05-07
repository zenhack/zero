#include <kernel/x86/8259pic.h>
#include <kernel/x86/portio.h>

/* resources: [8259], [osdev/PIC] */

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xa0
#define PIC_CMD(pic) (pic)
#define PIC_DATA(pic) ((pic)+1)

#define PIC_INIT 0x10
#define PIC_EOI 0x20

void remap_8259pic(void) {
	out8(PIC_CMD(PIC_MASTER), PIC_INIT);
	out8(PIC_CMD(PIC_SLAVE), PIC_INIT);
	out8(PIC_DATA(PIC_MASTER), IRQ(0)); /* Set the starting intterupt of the master */
	out8(PIC_DATA(PIC_SLAVE), IRQ(8)); /* ...and the slave. */

	/* Tell the pics about their relationship. The slave is on the master's
	 * pin #2. */
	out8(PIC_DATA(PIC_MASTER), 1<<2); /* The master expects a bitmask of slaves */
	out8(PIC_DATA(PIC_SLAVE), 2); /* The slave expects the master irq number it's
					 attached to (a 3 bit field). */
}

void disable_8259pic(void) {
	/* Okay, now actually disable the pic: */
	out8(PIC_DATA(PIC_MASTER), 0xff);
	out8(PIC_DATA(PIC_SLAVE), 0xff);
}
