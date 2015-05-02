all: $(objdir)/kernel.x86.elf

# Boot the kernel in qemu:
qemu-run: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio
# Same thing, but wait for gdb to attach:
qemu-gdb: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio -s -S
