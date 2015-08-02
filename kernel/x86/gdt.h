#ifndef KERNEL_X86_GDT_H
#define KERNEL_X86_GDT_H

#define NULL_SEGMENT 0
#define KCODE_SEGMENT 1
#define KDATA_SEGMENT 2
#define SEGOFF(seg) (8 * seg)

#ifndef ASM_FILE

typedef struct GDTDesc GDTDesc;
extern GDTDesc gdt_desc;
void gdt_load(GDTDesc *);

#endif
#endif
