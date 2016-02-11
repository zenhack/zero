#ifndef KERNEL_X86_MP_SETUP_H
#define KERNEL_X86_MP_SETUP_H

/* Start multiprocessor initialization. This should be called by the bootstrap
 * processor. It will detect the number of logical cpus in the system, record
 * this number in `num_cpus`, allocate space for their stacks, (storing a pointer
 * in `ap_stacks`), and start the other processors executing the function
 * `ap_start`. */
void mp_setup(void);

/* These are here primarily for documentation purposes, as they're only really
 * used from assembly in boot.S. num_cpus is declared as an int32_t so it's
 * obvious what the exact size is; things like size_t aren't usable from
 * assembly, so this makes things a bit more readable.
 *
 * ap_stacks points to a memory region that the APs will use for stack space.
 * It's big enough for each AP to have a 4K stack. Note that we're actually
 * wasting a bit of space, because it also includes space for a stack for the
 * BSP, which isn't needed. I (isd) *suspect* we can rely on the BSP always being
 * LAPIC #0, but for right now we're not relying on that. At some point we
 * should do the research. */
extern volatile int32_t num_cpus;
extern volatile void *ap_stacks;


#endif
