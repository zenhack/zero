#include <kernel/x86/gdt.h>
#include <stdint.h>

#define ACCESS_PRESENT (1<<7)
#define ACCESS_PRIVL(ring) (ring<<5)
#define ACCESS_EX (1<<3)
#define ACCESS_RW (1<<1)
#define ACCESS_ALWAYS1 (1<<4)

#define FLAG_GR_PAGE (1<<3)
#define FLAG_SZ_32 (1<<2)

/**
 * Expands to a struct literal describing a GDTEnt with the given parameters.
 * This lets us define the GDT statically in the binary.
 *
 * Note: Some of the arguments are used more than once within the body of the
 * macro, so only constant expressions may be used.
 */
#define MKGDT(base, limit, access_bits, flags) \
	{ \
		.limit_low = ((limit) & 0xffff), \
		.base_low = ((base) & 0xffff), \
		.base_mid = ((base) >> 16) & 0xff, \
		.access = (access_bits) | ACCESS_ALWAYS1, \
		.flags_limit_high = ((flags) << 4) | (((limit) >> 16) & 0x0f), \
		.base_high = ((base) >> 24) & 0xff \
	}

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

GDTEnt gdt[NUM_GDTENT] = {
	MKGDT(0, 0, 0, 0),
	MKGDT(0, ~0,
		ACCESS_PRESENT|ACCESS_PRIVL(0)|ACCESS_RW|ACCESS_EX,
		FLAG_GR_PAGE|FLAG_SZ_32),
	MKGDT(0, ~0,
		ACCESS_PRESENT|ACCESS_PRIVL(0)|ACCESS_RW,
		FLAG_GR_PAGE|FLAG_SZ_32),
};
GDTDesc gdt_desc = {
	.size = NUM_GDTENT * sizeof(GDTEnt) - 1,
	.offset = (uint32_t)&gdt[0],
};

/* Actually loads the gdt. in gdt_load.S */
extern void gdt_load(GDTDesc *desc);

void gdt_init(void) {
	gdt_load(&gdt_desc);
}
