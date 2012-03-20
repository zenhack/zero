#include <kernel/x86/gdt.h>

typedef struct GDTDesc GDTDesc;
typedef struct GDTEnt GDTEnt;

struct GDTDesc {
	uint16_t size;	
	uint32_t offset;
}__attribute__((packed));

struct GDTEnt {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags_limit_high;
	uint8_t base_high
}__attribute__((packed));

GDTDesc gdt_desc;
GDTEnt gdt[NUM_GDTENT];

/* Actually loads the gdt. in gdt_load.S */
extern void gdt_load(GDTDesc *desc);

void set_gdtent(GDTEnt *ent, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	ent->limit_low = (uint16_t)(limit & 0xffff);
	ent->base_low = (uint16_t)(base & 0xffff);
	ent->base_mid = (uint8_t)((base >> 16) & 0xff);
	ent->access = access;
	ent->flags_limit_high = (flags << 4) | (uint8_t)((limit >> 16) & 0xff);
	ent->base_high = (uint8_t)((base >> 24) & 0xff);
}

void gdt_init(void) {
	set_gdtent(&gdt[NULL_SEGMENT], 0, 0, 0, 0);
	set_gdtent(&gdt[KCODE_SEGMENT], 0, ~0, FOO, BAR);
	set_gdtentt(&gdt[KDATA_SEGMENT], 0, ~0, FOO, BAR);

	gdt_desc.size = NUM_GDTENT * sizeof(GDTEnt) - 1;
	gdt_desc.offset = (uint32_t)&gdt[0];

	gdt_load(&gdt_desc);
}
