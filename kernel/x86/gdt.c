#include <kernel/x86/gdt.h>
#include <stdint.h>

#define ACCESS_PRESENT (1<<7)
#define ACCESS_PRIVL(ring) (ring<<5)
#define ACCESS_EX (1<<3)
#define ACCESS_RW (1<<1)
#define ACCESS_ALWAYS1 (1<<4)

#define FLAG_GR_PAGE (1<<3)
#define FLAG_SZ_32 (1<<2)

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
	uint8_t base_high;
}__attribute__((packed));

GDTDesc gdt_desc;
GDTEnt gdt[NUM_GDTENT];

/* Actually loads the gdt. in gdt_load.S */
extern void gdt_load(GDTDesc *desc);

void set_gdtent(GDTEnt *ent, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	ent->limit_low = (uint16_t)(limit & 0xffff);
	ent->base_low = (uint16_t)(base & 0xffff);
	ent->base_mid = (uint8_t)((base >> 16) & 0xff);
	ent->access = access | ACCESS_ALWAYS1;
	ent->flags_limit_high = (flags << 4);
	ent->flags_limit_high |= (uint8_t)((limit >> 16) & 0x0f);
	ent->base_high = (uint8_t)((base >> 24) & 0xff);
}

void gdt_init(void) {
	set_gdtent(&gdt[NULL_SEGMENT], 0, 0, 0, 0);
	set_gdtent(&gdt[KCODE_SEGMENT], 0, ~0,
		ACCESS_PRESENT|ACCESS_PRIVL(0)|ACCESS_RW|ACCESS_EX,
		FLAG_GR_PAGE|FLAG_SZ_32);
	set_gdtent(&gdt[KDATA_SEGMENT], 0, ~0,
		ACCESS_PRESENT|ACCESS_PRIVL(0)|ACCESS_RW,
		FLAG_GR_PAGE|FLAG_SZ_32);

	gdt_desc.size = NUM_GDTENT * sizeof(GDTEnt) - 1;
	gdt_desc.offset = (uint32_t)&gdt[0];

	gdt_load(&gdt_desc);
}
