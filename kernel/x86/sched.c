#include <kernel/x86/sched.h>
#include <kernel/x86/thread.h>

static Queue ready;

static Thread *running;

static Thread k_idle;

void sched_insert(Thread *t) {
	enq(&ready, (List *)t);
}

Regs *sched(Regs *old_ctx) {
	if(running == NULL) {
		running = &k_idle;
	}
	running->ctx = old_ctx;
	enq(&ready, (List *)running);
	running = (Thread *)deq(&ready);
	return running->ctx;
}
