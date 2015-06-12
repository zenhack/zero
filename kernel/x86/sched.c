#include <kernel/x86/sched.h>
#include <kernel/x86/thread.h>

static Thread *head, *tail;
static Thread *running;

void sched_insert(Thread *thread) {
	thread->next = NULL;
	if(head == NULL) {
		head = tail = thread;
	} else {
		tail->next = thread;
	}
}

Thread *sched_remove(void) {
	Thread *ret = head;
	head = head->next;
	if(head == NULL) {
		tail = NULL;
	}
	ret->next = NULL;
	return ret;
}

void sched(Regs *regs) {
	if(running != NULL) {
		running->regs = *regs;
		sched_insert(running);
	}
	running = sched_remove();
	*regs = running->regs;
}
