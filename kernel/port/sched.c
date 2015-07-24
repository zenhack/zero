#include <stddef.h>
#include <kernel/port/sched.h>

static Queue ready;
static Thread k_idle;
static Thread *running = &k_idle;

void sched_insert(Thread *t) {
	enq(&ready, (List *)t);
}

void *sched(void *old_ctx) {
	running->ctx = old_ctx;
	if(running != &k_idle) {
		enq(&ready, (List *)running);
	}

	running = (Thread *)deq(&ready);
	if(running == NULL) {
		running = &k_idle;
	}
	return running->ctx;
}
