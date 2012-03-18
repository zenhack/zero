/*
	Constants for multiboot.
*/
#ifndef KERN_X86_MULTIBOOT_H
#define KERN_X86_MULTIBOOT_H

#define MULTIBOOT_MAGIC 0x1badb002
#define MULTIBOOT_CHKSUM(flags) (-(MULTIBOOT_MAGIC + flags))

#define MULTIBOOT_MEMINFO (1<<1)

#ifndef ASM_FILE

typedef struct MultiBootInfo MultiBootInfo;
struct MultiBootInfo {
	long flags;
	long mem_lower;
	long mem_upper;
	/* there are more fields, but we aren't interested. (yet.) */
};

#endif
#endif
