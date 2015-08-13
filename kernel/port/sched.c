#include <stddef.h>
#include <kernel/port/sched.h>

/* FIXME: We must *not* be including architecture specific headers from
 * portable souce files. The source tree/build system needs to be restructured
 * so that we can have architecture-specific implementations of portable
 * interfaces. */
#include <kernel/x86/lock.h>

static Queue ready;
static mutex_t ready_lock;
static Thread k_idle;
static Thread *running = &k_idle;

void sched_insert(Thread *t) {
	enq(&ready, (List *)t);
}

void *sched(void *old_ctx) {
	wait_aquire(&ready_lock);
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
