#ifndef KERNEL_ARCH_LOCK_H
#define KERNEL_ARCH_LOCK_H

#include <stdint.h>
#include <kernel/port/panic.h>

typedef uint32_t mutex_t;

static inline
uint32_t try_acquire(mutex_t *mutex) {
	return __sync_bool_compare_and_swap(mutex, 0, 1);
}

static inline
void wait_acquire(mutex_t *mutex) {
	while(!try_acquire(mutex));
}

static inline
void release(mutex_t *mutex) {
	if (!__sync_bool_compare_and_swap(mutex, 1, 0)) {
		panic("BUG: Called release() on an unlocked mutex!");
	}
};

#endif
