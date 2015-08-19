#ifndef KERNEL_X86_COTHREAD_H
#define KERNEL_X86_COTHREAD_H

#include <stddef.h>

/* creates a new cothread with a stack of size `stack_size`, ready to execute
 * the function `entry`, which must not return. the argument passed to entry
 * is a pointer to the cothread which yields to it. */
void *mk_thread(size_t stack_size, void (*entry)(void *));

/* yields to the cothread pointed to by `to`. Returns a pointer to the
 * cothread which yielded to the calling thread. */
void *yield(void *to);

#endif
