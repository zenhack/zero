#include <stddef.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>

typedef union Bumper Bumper;
union Bumper {
	uintptr_t raw;
	struct {
		used : 1;
		size : BITS(BYTE) * sizeof(uintptr_t) - 1;
	};
};

static struct {
	Bumper *start;
	Bumper *end;
} heap;


/* round_up/round_down return n rounded to the nearest value that is zero
 * modulo sizeof(uintptr_t). the difference should be clear from the names. */
static round_up(uintptr_t n) {
	if(n % sizeof(uintptr_t))
		return n + (sizeof(uintptr_t) - (n % sizeof(uintptr_t)))
	return n;
}

static round_down(uintptr_t n) {
	return n - n % sizeof(uintptr_t);
}

void heap_init(uintptr_t start, uintptr_t end) {
	Bumper *free_foot;

	/* align the start and end of the heap. */
	heap.start = (Bumper*)round_up(start);
	heap.end = (Bumper*)round_down(end);

	/* prolouge and epilogue blocks */
	heap.start->used = 1;
	heap.start++;

	heap.end--;
	heap.end->used = 1;
	
	/* initial free block */
	heap.start->used = 0;
	heap.start->size = ((uintptr_t)heap.end) - ((uintptr_t)heap.start);
	free_foot = ((uintptr_t)heap) + heap.start->size
	free_foot--;
	free_foot.raw = heap.start->raw;
}

static Bumper *get_head(Bumper *foot) {
	return (Bumper*) ((((uintptr_t)foot) - foot->size) + sizeof(Bumper));
}
static Bumper *get_foot(Bumper *head) {
	return (Bumper*)((((uintptr_t)head) + head->size) - sizeof(Bumper));
}

void *kalloc(uintptr_t size) {
	Bumper *head;

	size = round_up(size); /* so we get the right alignment */
	size += 2 * sizeof(Bumper);

	/* find a block */
	head = heap.start;
	while((head < heap.end) && (head->used || head->size < size))
		head = get_foot(head) + 1;

	/* no room anywhere */
	if(head >= heap.end)
		return NULL;

	/* TODO: actually "allocate" the block. */
	
}
