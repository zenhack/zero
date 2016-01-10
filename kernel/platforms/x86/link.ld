ENTRY(start)

/* Make sure we don't overflow the region of memory we're loading into.
 * The region is 408.5K, but ld's syntax won't do fractional values, hence the
 * awkward specification: */
MEMORY { kimage (rwx): ORIGIN = 0x7e00, LENGTH = 408K + 512 }

SECTIONS
{

	/* There's an area of ram guaranteed to be present and free for
	 * use starting at 0x7e00, which is 480.5 KiB in size
	 * [osdev/Memory_Map]. We load our kernel at the first page boundary
	 * within that region.
	 *
	 * The kernel needs to be loaded below the 1 MiB mark, so that
	 * additional CPUs can jump to something inside the kernel image when
	 * they are started. */
	. = 0x7e00;
	. = ALIGN(4096);
	.text :
	{
		code = .; _code = .; __code = .;
		*(.text)
		. = ALIGN(4096);
	} >kimage
	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	} >kimage
	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	} >kimage
	kend = .; _kend = .; __kend = .;
}

