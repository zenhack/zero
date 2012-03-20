#ifndef KERNEL_X86_GDT_H
#define KERNEL_X86_GDT_H

#define NUM_GDTENT 3
#define NULL_SEGMENT 0
#define KCODE_SEGMENT 1
#define KDATA_SEGMENT 2
#define SEGOFF(seg) (8 * seg)

#ifndef ASM_FILE

void gdt_init(void);

#endif
#endif
