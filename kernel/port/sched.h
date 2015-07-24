#ifndef KERNEL_PORT_SCHED_H
#define KERNEL_PORT_SCHED_H

#include <kernel/port/data.h>

typedef struct Thread Thread;
struct Thread {
	List l;
	void *ctx;
};

void sched_insert(Thread *new_thread);
void *sched(void *old_ctx);

#endif
