#ifndef KERNEL_X86_MP_SETUP_H
#define KERNEL_X86_MP_SETUP_H

#include <kernel/port/data.h>
#include <kernel/arch/lock.h>

/* Start multiprocessor initialization. This should be called by the bootstrap
 * processor. It will detect the number of logical cpus in the system and allocate
 * space for their stacks, storing a pointer to a list of unused stacks in
 * `ap_stacks`. It will then start the other processors executing the function
 * `ap_start`. */
void mp_setup(void);

extern mutex_t ap_stack_lock;
extern volatile List *ap_stacks;


#endif
