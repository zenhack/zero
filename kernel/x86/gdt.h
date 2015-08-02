#ifndef KERNEL_X86_GDT_H
#define KERNEL_X86_GDT_H

/**
 * Support for managing the Global Descriptor Table (GDT)
 *
 * GDT initialization in zero is trivial at runtime: the entire structure is
 * baked into the binary, ready to go, and the bringup code need only load the
 * GDT pointer. A simple call to:
 *
 *     gdt_load(&gdt_desc);
 *
 * Will do the trick.
 */

/* symbolic constants for specific segments, as used by our kernel */
#define NULL_SEGMENT 0
#define KCODE_SEGMENT 1
#define KDATA_SEGMENT 2

/* The offset from the beginning of the GDT of a particular segment
 * descriptor. */
#define SEGOFF(seg) (8 * seg)

#ifndef ASM_FILE

/* The GDT pointer structure. defined in gdt.c */
typedef struct GDTDesc GDTDesc;
extern GDTDesc gdt_desc;

/* Load the GDT pointer supplied as an argument, then start using it. defined in
 * gdt_load.S */
void gdt_load(GDTDesc *);

#endif
#endif
