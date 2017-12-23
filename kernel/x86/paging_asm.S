.globl set_page_dir
set_page_dir:
	mov 4(%esp), %eax
	mov %eax, %cr3
	ret

.globl enable_paging
enable_paging:
	mov %cr0, %eax
	orl %eax, (1<<31)
	mov %eax, %cr0
	ret
