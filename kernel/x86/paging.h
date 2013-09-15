#ifndef KERNEL_X86_PAGING_H
#define KERNEL_X86_PAGING_H

#include <stdint.h>

typedef struct PageEntry PageEntry;
typedef struct PageStruct PageStruct;

struct PageEntry {
	uint32_t present : 1;
	uint32_t rw : 1;
	uint32_t user : 1;
	uint32_t write_through: 1;
	uint32_t cache_disable : 1;
	uint32_t access : 1;
	uint32_t dirty : 1;
	uint32_t size : 1;
	uint32_t global : 1;
	uint32_t avail : 2;
	uint32_t frame : 20;
} __attribute__((packed));

struct PageStruct {
	PageEntry *ents[1024];
};

/* Sets the current page directory to the page struct located at `pdir`.
   This merely loads the address into CR3; It doesn't do anything else, e.g.
   enable paging. */
void set_page_dir(PageStruct *pdir);



#endif
