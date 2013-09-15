.globl set_page_dir
set_page_dir:
	mov 4(%esp), %eax
	mov %eax, %cr3
	ret
