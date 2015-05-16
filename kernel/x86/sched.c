#include <kernel/x86/sched.h>
#include <kernel/x86/thread.h>

static Thread *head, *tail;
static int started = 0;

void sched_insert(Thread *thread) {
	thread->next = NULL;
	if(head == NULL) {
		head = tail = thread;
	} else {
		tail->next = thread;
	}
}

void sched(Regs *regs) {
	Thread *old;
	if(!started) {
		head->regs = *regs;

		old = head;
		head = head->next;

		old->next = NULL;
		tail->next = old;
		tail = old;
		started = 1;
	}

	*regs = head->regs;
}
