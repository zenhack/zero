#include <stdint.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/isr.h>
#include <kernel/x86/gdt.h>
#include <kernel/port/stdio.h>
#include <kernel/port/panic.h>

#define IDT_ATTR_PRESENT (1<<7)
#define IDT_ATTR_DPL(ring) (ring<<5)
#define IDT_ATTR_STORAGE (1<<4)

#define IDT_TYP_TASK32 0x5
#define IDT_TYP_INT16 0x6
#define IDT_TYP_TRAP16 0x7
#define IDT_TYP_INT32 0xe
#define IDT_TYP_TRAP32 0xf

typedef struct IDTDesc IDTDesc;
typedef struct IDTEnt IDTEnt;

struct IDTDesc {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct IDTEnt {
	uint16_t offset_low;
	uint16_t select;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
}__attribute__((packed));

IDTDesc idt_desc;
IDTEnt idt[NUM_IDT];

static int_handler handlers[NUM_IDT];

void register_int_handler(uint8_t num, int_handler h) {
	handlers[num] = h;
}

void set_isr(IDTEnt *ent, uintptr_t isr) {
	ent->offset_low = isr & 0xffff;
	ent->offset_high = (isr >> 16) & 0xffff;
	ent->select = SEGOFF(KCODE_SEGMENT);
	ent->zero = 0;
	ent->type_attr = IDT_ATTR_PRESENT|IDT_ATTR_DPL(0)|IDT_TYP_INT32;
}

/* in lidt.S */
extern void lidt(IDTDesc *);

void idt_init(void) {
	int i;

	idt_desc.size = NUM_IDT * sizeof(IDTEnt) - 1;
	idt_desc.offset = (uint32_t)&idt[0];

	for(i = 0; i < NUM_IDT; i++)
		set_isr(&idt[i], (uintptr_t)isrs[i]);

	lidt(&idt_desc);
}

void int_handler_common(Regs *regs) {
	int_handler h = handlers[regs->int_no];
	if(h)
		h(regs);
	else
		panic("Unhandled Interrupt!");
}
