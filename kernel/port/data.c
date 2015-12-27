#include <kernel/port/data.h>
#include <stdint.h>

void enq(Queue *q, List *item) {
	item->next = NULL;
	if(!q->tail) {
		q->head = item;
		q->tail = item;
	} else {
		q->tail->next = item;
		q->tail = item;
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

List *sew_list(void *buf, size_t buf_size, size_t elt_size) {
	size_t num_elts = buf_size / elt_size;
	List *head = buf;
	head->next = NULL;
	for(size_t i = 0; i < num_elts - 1; i++) {
		uintptr_t addr = (uintptr_t)head;
		List *new_head = (List *)(addr + elt_size);
		new_head->next = head;
		head = new_head;
	}
	return head;
}
