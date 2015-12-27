#ifndef KERNEL_PORT_DATA_H
#define KERNEL_PORT_DATA_H

#include <stddef.h>

/**
 * Generic data structures.
 *
 * So far:
 *
 * - Singly-linked lists (List)
 * - Queues (Queue)
 *
 * Note that the List struct is meant to be embedded in the data items, not the
 * other way around:
 *
 * struct MyListElement {
 * 	List list;
 * 	int foo;
 * 	char bar;
 * 	void *baz;
 * 	...
 * };
 */

typedef struct List List;
typedef struct Queue Queue;

struct List {
	List *next;
};

struct Queue {
	List *head, *tail;
};

/* Insert an item at the end of the queue. */
void enq(Queue *q, List *item);

/* Remove an item from the front of the queue, and return it. If the queue is
 * empty, return NULL. */
List *deq(Queue *q);


/* "Sews" a list of elements of size `elt_size` through the buffer located at
 * `buf`, which must be of size `buf_size`.
 *
 * Returns a pointer to the head of a list of as many elements of size
 * `elt_size` as will fit in the alotted space. */
List *sew_list(void *buf, size_t buf_size, size_t elt_size);

#endif
