ENTRY(_start)
SECTIONS {
	.text 0x8000 :
	{
		*(.text)
		*(.data)
		*(.rodata)
		*(.bss)
	}
}
