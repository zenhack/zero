/*
	Constants for multiboot.
*/
#ifndef KERN_X86_MULTIBOOT_H
#define KERN_X86_MULTIBOOT_H

#define MULTIBOOT_MAGIC 0x1badb002
#define MULTIBOOT_CHKSUM(flags) (-(MULTIBOOT_MAGIC + flags))

#endif
