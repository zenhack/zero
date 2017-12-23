#include <kernel/x86/paging.h>
#include <kernel/port/heap.h>

static PageStruct initial_page_dir;

/* in paging_asm.S */
extern void enable_paging(void);

int paging_init(uintptr_t max_addr) {
	uintptr_t i, j = 0;
	/* for each page table */
	for(i = 0; (i<<10) + j < (max_addr >> 12); i += 1<<10) {
		PageStruct *page_table =  kalloc_align(PAGESIZE, PAGESIZE);
		if(!page_table) {
			return -1;
		}
		initial_page_dir.ents[i].frame = ((uintptr_t)page_table) >> 12;
		initial_page_dir.ents[i].present = 1;
		initial_page_dir.ents[i].rw = 1;
		initial_page_dir.ents[i].user = 0;
		initial_page_dir.ents[i].write_through = 0;
		initial_page_dir.ents[i].cache_disable = 0;
		initial_page_dir.ents[i].size = 0;
		/* for each page */
		for (j = 0; (i<<10) + j < (max_addr >> 12)
			&& j < PAGESIZE/sizeof(PageEntry); j++) {

			page_table->ents[j].frame = (i << 10) + j;
			page_table->ents[j].present = 1;
			page_table->ents[j].rw = 1;
			page_table->ents[j].user = 0;
			page_table->ents[j].write_through = 0;
			page_table->ents[j].cache_disable = 0;
			page_table->ents[j].size = 0;
		}
	}

	set_page_dir(&initial_page_dir);

	enable_paging();
	return 0;
}
