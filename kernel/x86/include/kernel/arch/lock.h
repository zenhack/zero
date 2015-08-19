#ifndef KERNEL_ARCH_LOCK_H
#define KERNEL_ARCH_LOCK_H

#include <kernel/x86/asm.h>
#include <kernel/port/panic.h>

typedef uint32_t mutex_t;

static inline
uint32_t try_aquire(mutex_t *mutex) {
	return !cmpxchg(0, mutex, 1);
}

static inline
void wait_aquire(mutex_t *mutex) {
	while(!try_aquire(mutex));
}

static inline
void release(mutex_t *mutex) {
	uint32_t old = cmpxchg(1, mutex, 0);
	if(!old) {
		panic("BUG: Called release() on an unlocked mutex!");
	}
};

#endif
