#ifndef KERNEL_ARCH_LOCK_H
#define KERNEL_ARCH_LOCK_H

#include <kernel/rpi/asm.h>
#include <kernel/port/panic.h>

typedef uint32_t mutex_t;

static inline
uint32_t try_acquire(mutex_t *mutex) {
	if(ldrex(mutex) != 0) return 1;
	return !strex(mutex, 1);
}

static inline
void wait_acquire(mutex_t *mutex) {
	while(!try_acquire(mutex));
}

static inline
void release(mutex_t *mutex) {
	if(ldrex(mutex) != 1) {
		panic("BUG: Called release() on an unlocked mutex!");
	}
	// TODO: verify that this works; we aren't checking for failure
	// here, but nothing else should be calling release(), so
	// external modifications should never happen. For the below to
	// be correct that has to imply that the strex can't fail.
	strex(mutex, 0);
};

#endif
