#include <stddef.h>
#include <kernel/port/heap.h>
#include <kernel/port/units.h>

typedef struct Bumper Bumper;
struct Bumper {
	uintptr_t used : 1;
	uintptr_t size : BITS(sizeof(uintptr_t)) - 1;
};

static struct {
	Bumper *start, *end;
} heap;

static uintptr_t round_down(uintptr_t n, uintptr_t align) {
	return n - (n % align);
}

static uintptr_t round_up(uintptr_t n, uintptr_t align) {
	if (n % align == 0) {
		return n;
	}
	return round_down(n, align) + align;
}

static Bumper *get_foot(Bumper *head) {
	return (Bumper *)((uintptr_t)head + head->size) - 1;
}

static Bumper *get_head(Bumper *foot) {
	return (Bumper *)((uintptr_t)foot - foot->size) + 1;
}

static Bumper *align_head(Bumper *head, uintptr_t align) {
	return (Bumper *)round_up((uintptr_t)head, align) - 1;
}

static int has_aligned_space(Bumper *head, uintptr_t size, uintptr_t align) {
	Bumper *aligned_head = align_head(head, align);
	uintptr_t size_loss = (uintptr_t)aligned_head - (uintptr_t)head;
	return head->size - size_loss >= size;
}

static void make_block(Bumper *head, Bumper *foot, uintptr_t used) {
	uintptr_t size = (uintptr_t)(foot + 1) - (uintptr_t)head;
	head->size = size;
	head->used = used;
	*foot = *head;
}

void heap_init(uintptr_t start, uintptr_t end) {
	/* make sure the heap is properly aligned */
	heap.start = (Bumper *)round_up((uintptr_t)start, sizeof(Bumper));
	heap.end   = (Bumper *)round_down((uintptr_t)end, sizeof(Bumper));

	/* Bound the heap with things that look like "used" blocks; this way we
	 * don't need any special case logic when merging adjacent blocks on a
	 * free. */
	heap.start->used = 1;
	heap.start++;
	heap.end->used = 1;
	heap.end--;

	/* Now make the rest one giant free block */
	make_block(heap.start, heap.end, 0);
}

void _kfree(void *ptr) {
	Bumper *head = (Bumper *)ptr - 1;
	Bumper *foot = get_foot(head);

	Bumper *left_foot = foot - 1;
	Bumper *right_head = foot + 1;

	if(!left_foot->used) {
		head = get_head(left_foot);
	}
	if(!right_head->used) {
		foot = get_foot(right_head);
	}

	make_block(head, foot, 0);
}

void kfree(void *ptr, uintptr_t size) {
	return _kfree(ptr);
}


void *kalloc_align(uintptr_t size, uintptr_t align) {
	/* regardless of what the user asks for, we always need to align at
	 * least on a bumper-sized boundary. This also means we're allocating
	 * whole-bumper sized objects. */
	size  = round_up(size, sizeof(Bumper));
	align = round_up(size, sizeof(Bumper));

	/* make space for the bumpers themselves: */
	size += 2 * sizeof(Bumper);

	/* find the first block that will fit */
	Bumper *head = heap.start;
	while(head < heap.end && !has_aligned_space(head, size, align)) {
		head = get_foot(head) + 1;
	}
	if(head >= heap.end) {
		/* Out of luck */
		return NULL;
	}

	/* align the head */
	Bumper *foot = get_foot(head);
	Bumper *aligned_head = align_head(head, align);

	/* turn any leading excess into it's own (free) block. Note that it is
	 * possible for this block to be too small to be usable, but it doesn't
	 * cause a correctness problem. Even if the head and foot end up being
	 * the same piece of memory, everything still works. */
	if(head != aligned_head) {
		Bumper *left_foot = aligned_head - 1;
		make_block(head, left_foot, 0);
	}

	/* set up the footer at the right place. */
	aligned_head->size = size;
	Bumper *aligned_foot = get_foot(aligned_head);
	make_block(aligned_head, aligned_foot, 1);

	/* Same thing, turn trailing excess into its own block. */
	if(foot != aligned_foot) {
		Bumper *right_head = aligned_foot + 1;
		make_block(right_head, foot, 0);
	}
	return aligned_head + 1;
}

void *kalloc(uintptr_t size) {
	return kalloc_align(size, sizeof(uintptr_t));
}
