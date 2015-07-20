#ifndef KERNEL_X86_SCHED_H
#define KERNEL_X86_SCHED_H

#include <kernel/x86/regs.h>
#include <kernel/x86/thread.h>

void sched_insert(Thread *new_thread);
Regs *sched(Regs *old_ctx);

#endif
