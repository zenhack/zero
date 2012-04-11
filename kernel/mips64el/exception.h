#ifndef KERNEL_MIPS64EL_EXCEPTION_H
#define KERNEL_MIPS64EL_EXCEPTION_H

#define RESET_NMI_VEC        0xffffffffbfc00000
#define BEV0_TLB_REFILL_VEC  0xffffffff80000000
#define BEV0_XTLB_REFILL_VEC 0xffffffff80000000
#define BEV0_CACHE_ERROR_VEC 0xffffffffa0000100
#define BEV0_OTHERS_VEC      0xffffffff80000180
#define BEV1_TBL_REFILL_VEC  0xffffffffbfc00200
#define BEV1_XTBL_REFILL_VEC 0xffffffffbfc00200
#define BEV1_CACHE_ERROR_VEC 0xffffffffbfc00300
#define BEV1_OTHERS_VEC      0xffffffffbfc00380

extern void *tlb_refill;

#endif

