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


/* round_up/round_down return `n` rounded to the nearest value that is zero
 * modulo `align`. the difference should be clear from the names. */
static uintptr_t round_up(uintptr_t n, uintptr_t align) {
	if (n % align) {
		return n + (align - (n % align));
	}
	return n;
}

static uintptr_t round_down(uintptr_t n, uintptr_t align) {
	return n - n % align;
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
	heap.start = (Bumper*)round_up(start, sizeof(uintptr_t));
	heap.end = (Bumper*)round_down(end, sizeof(uintptr_t));

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

/* Returns a true value if the block with header `head` is large enough to
 * store a value of size `space` on an `align` byte boundary. Returns false
 * otherwise. */
static int has_aligned_space(Bumper *head, uintptr_t space, uintptr_t align) {
	/* round up to the nearest `align` byte boundary, find the difference
	 * between that and the start of the block, and subtract that from the
	 * total size. */
	return 
		head->v.size
		- (
			round_up((uintptr_t)(head+1),align)
			- (uintptr_t)(head+1)
		) >= space;
}

void *kalloc_align(uintptr_t size, uintptr_t alignment) {
	Bumper *head, *foot, *new_foot, *new_head;

	/* we're not allocating units less than sizeof(uintptr_t); round up
	 * to the nearest such unit and adjust for the header/footer. */
	size = round_up(size, sizeof(uintptr_t));
	size += 2 * sizeof(Bumper);

	/* find a block */
	head = heap.start;
	while((head < heap.end) &&
		(head->v.used ||
		!has_aligned_space(head,size,alignment))) {

		head = get_foot(head) + 1;
	}

	/* no room for an allocation of this size. */
	if(head >= heap.end)
		return NULL;

	new_head = (Bumper*)round_up((uintptr_t)(head+1), alignment);
	new_head--;

	/* Start of block isn't right for alignment. */
	if(new_head != head) {
		/* we need to split the block so that kfree will find the
		 * bumpers. */
		foot = new_head - 1;
		head->v.size -= size;
		foot->raw = head->raw;

		new_head->v.size = size;
		new_head->v.used = 1;
		new_foot = get_foot(new_head);
		new_foot->raw = new_head->raw;
		return new_head + 1;
	}

	/* alignment is okay : */
	
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

void *kalloc(uintptr_t size) {
	return kalloc_align(size, sizeof(uintptr_t));
}

void kfree(void *ptr, uintptr_t size) {
	Bumper *head, *foot, *foot_left, *head_right;

	size = round_up(size, sizeof(uintptr_t));
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
