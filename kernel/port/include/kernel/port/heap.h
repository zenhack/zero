#ifndef KERNEL_PORT_HEAP_H
#define KERNEL_PORT_HEAP_H

#include <stdint.h>

/* Initializes a heap to allocate from addresses ranging from
   start to end. */
void heap_init(uintptr_t start, uintptr_t end);

/* returns a pointer to a newly allocated memory region of size `size`.
   returns NULL on failure. allocated region will always be aligned on a
   sizeof(uintptr_t) byte boundary. */
void *kalloc(uintptr_t size);

/* same as kalloc, except that the returned memory will be aligned on an
 * `alignment` byte boundary (with a minimum alignment of sizeof(uintptr_t)
 * bytes). */
void *kalloc_align(uintptr_t size, uintptr_t alignment);

/* frees a memory region previously allocated by kalloc. Note that this also
   expects the size - I have some thoughts about a more cache friendly
   allocator based on ideas from k42, so I may want that information in
   the future. For now the memory allocator doesn't need it, but can use it
   for sanity checking. */
void kfree(void *ptr, uintptr_t size);

#endif
