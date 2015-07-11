#include <kernel/x86/sched.h>
#include <kernel/x86/thread.h>

static Queue ready;

static Thread *running;

void sched_insert(Thread *t) {
	enq(&ready, (List *)t);
}

void sched(Regs *regs) {
	if(running != NULL) {
		running->regs = *regs;
		enq(&ready, (List *)running);
	}
	running = (Thread *)deq(&ready);
	if(running->regs.eflags == 0) {
		/* thread hasn't been scheduled before. Copy a few values
		 * from previous context: */
		running->regs.eflags = regs->eflags;
		running->regs.esp = regs->esp;
	}
	*regs = running->regs;
}
