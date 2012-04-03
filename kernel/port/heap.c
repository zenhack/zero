#include <stddef.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>
#include <kernel/port/stdio.h>
#include <kernel/port/panic.h>

typedef union Bumper Bumper;
union Bumper {
	uintptr_t raw;
	struct {
		uintptr_t used : 1;
		uintptr_t size : BITS(BYTE) * sizeof(uintptr_t) - 1;
	} v;
};

static struct {
	Bumper *start;
	Bumper *end;
} heap;


/* round_up/round_down return n rounded to the nearest value that is zero
 * modulo sizeof(uintptr_t). the difference should be clear from the names. */
static uintptr_t round_up(uintptr_t n) {
	if(n % sizeof(uintptr_t))
		return n + (sizeof(uintptr_t) - (n % sizeof(uintptr_t)));
	return n;
}

static uintptr_t round_down(uintptr_t n) {
	return n - n % sizeof(uintptr_t);
}

static Bumper *get_head(Bumper *foot) {
	return (Bumper*) ((((uintptr_t)foot) - foot->v.size) + sizeof(Bumper));
}

static Bumper *get_foot(Bumper *head) {
	return (Bumper*)((((uintptr_t)head) + head->v.size) - sizeof(Bumper));
}

void heap_init(uintptr_t start, uintptr_t end) {
	Bumper *free_foot;

	/* align the start and end of the heap. */
	heap.start = (Bumper*)round_up(start);
	heap.end = (Bumper*)round_down(end);

	/* prolouge and epilogue blocks */
	heap.start->v.used = 1;
	heap.start++;

	heap.end--;
	heap.end->v.used = 1;
	
	/* initial free block */
	heap.start->v.used = 0;
	heap.start->v.size = ((uintptr_t)heap.end) - ((uintptr_t)heap.start);
	free_foot = get_foot(heap.start);
	free_foot->raw = heap.start->raw;
}

void *kalloc(uintptr_t size) {
	Bumper *head, *foot, *new_foot, *new_head;

	size = round_up(size); /* so we get the right alignment */
	size += 2 * sizeof(Bumper);

	/* find a block */
	head = heap.start;
	while((head < heap.end) && (head->v.used || head->v.size < size))
		head = get_foot(head) + 1;

	/* no room for an allocation of this size. */
	if(head >= heap.end)
		return NULL;
	
	/* not enough room to split, just return the whole thing. */
	if(head->v.size <= size + 2 * sizeof(Bumper)) {
		head->v.used = 1;
		foot = get_foot(head);
		foot->raw = head->raw;
		return head + 1;
	}

	/* room for at least one byte; split the block. */
	foot = get_foot(head);
	foot->v.size -= size;
	head->v.size = size;
	new_head = get_head(foot);
	new_foot = get_foot(head);
	head->v.used = 1;
	new_foot->raw = head->raw;
	new_head->raw = foot->raw;
	return head + 1;
}

void kfree(void *ptr, uintptr_t size) {
	Bumper *head, *foot, *foot_left, *head_right;

	size = round_up(size);
	size += 2 * sizeof(Bumper);

	head = ptr;
	head--;

	/* sanity checks */
	if(!head->v.used) {
		panic("double free");
	}
	if(head->v.size < size || head->v.size > size + 2 * sizeof(Bumper)) {
		panic("size mismatch in free!");
	}

	foot = get_foot(head);

	foot_left = head - 1;
	head_right = foot + 1;

	if(!foot_left->v.used) {
		size += foot_left->v.size;
		head = get_head(foot_left);
	}

	if(!head_right->v.used) {
		size += head_right->v.size;
		foot = get_foot(head_right);
	}

	head->v.used = 0;
	head->v.size = size;
	foot->raw = head->raw;
}
