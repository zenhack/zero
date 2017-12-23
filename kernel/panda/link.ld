ENTRY(_start)
SECTIONS {
	/* 0x8000 - 8 (for gp header) */
	.text 0x7ff8 :
	{
		*(.text)
		*(.data)
		*(.rodata)
		*(.bss)
	}
	_end = .;
}
