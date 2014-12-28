#ifndef KERNEL_PORT_BITWHACK_H
#define KERNEL_PORT_BITWHACK_H

/* Helpers for bitwhacking. */

/**
 * Macros for constructing bit masks.
 *
 * MASK_LO(n) masks out the lowest n bits.
 * MASK_HI(n) masks out all but the lowest n bits.
 * MASK_RANGE(lo, hi) masks out all of the bits on the interval [lo, hi] 
 * (inclusive).
 *
 * The KEEP_* macros produce masks that are the inverse of their MASK_* 
 * counterparts.
 */
#define MASK_LO(bits) ((-1)<<(bits))
#define KEEP_LO(bits) (~MASK_LO(bits))
#define MASK_HI(bits) KEEP_LO(bits)
#define KEEP_HI(bits) MASK_LO(bits)
#define KEEP_RANGE(lo, hi) (MASK_LO(lo) & MASK_HI(hi))
#define MASK_RANGE(lo, hi) (~KEEP_RANGE(lo, hi))

#endif
