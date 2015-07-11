#include <stddef.h>
#include <kernel/port/data.h>

void enq(Queue *q, List *item) {
	item->next = NULL;
	if(!q->tail) {
		q->head = item;
		q->tail = item;
	} else {
		q->tail->next = item;
	}
}

List *deq(Queue *q) {
	if(!q->head) {
		return NULL;
	}
	List *ret = q->head;
	q->head = q->head->next;
	if(!q->head) {
		q->tail = NULL;
	}
	ret->next = NULL;
	return ret;
}
