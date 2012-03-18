ENTRY(start)

SECTIONS
{

	/* start of high-memory, right ater the first block of io ports. */
	.text 0x100000 :
	{
		code = .; _code = .; __code = .;
		*(.text)
		. = ALIGN(4096);
	}
	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	kend = .; _kend = .; __kend = .;
}

