// The start of the file should be the gp header, which contains the
// minimal information needed to load the kernel. This includes the size
// of the image, and the load address.
//
// A few things that I need to double check:
//
// 1. Does the size include the 8 bytes for the gp header?
// 2. Does the entire image get loaded at the load address, or just the
//    executable?
//
// The code below assumes the size includes the header, and that the
// header is not loaded with the rest of the kernel.
_gp_header$:
	.int 0      // Size of kernel image, patched in later by build system.
	.int _start // kernel entry point
.globl _start
_start:
	mov sp, #_start // put the stack right below our entry point.
	mov fp, sp
	bl arch_main
hang$:
	b hang$
