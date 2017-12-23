#include <stddef.h>
#include <kernel/port/sched.h>
#include <kernel/port/lock.h>

static Queue ready;
static mutex_t ready_lock;
static Thread k_idle;
static Thread *running = &k_idle;

void sched_insert(Thread *t) {
	wait_acquire(&ready_lock);
	enq(&ready, (List *)t);
	release(&ready_lock);
}

void *sched(void *old_ctx) {
	wait_acquire(&ready_lock);
	running->ctx = old_ctx;
	if(running != &k_idle) {
		enq(&ready, (List *)running);
	}

	running = (Thread *)deq(&ready);
	if(running == NULL) {
		running = &k_idle;
	}
	void *ret = running->ctx;
	release(&ready_lock);
	return ret;
}
