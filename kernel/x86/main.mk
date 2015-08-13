all: kernel.x86.elf

ssrc += \
	kernel/x86/boot.S\
	kernel/x86/cpuid.S\
	kernel/x86/gdt_load.S\
	kernel/x86/isr_stub.S\
	kernel/x86/lidt.S\
	kernel/x86/paging_asm.S

csrc += \
	kernel/x86/8259pic.c\
	kernel/x86/apic.c\
	kernel/x86/apic_timer.c\
	kernel/x86/arch_main.c\
	kernel/x86/gdt.c\
	kernel/x86/idt.c\
	kernel/x86/paging.c\
	kernel/x86/panic.c\
	kernel/x86/pit.c\
	kernel/x86/serial.c\
	kernel/x86/text_console.c\
	kernel/x86/thread.c\
	\
	kernel/x86/isr_gen.c

cleanfiles += \
	kernel/x86/isr_gen.c

kernel/x86/isr_gen.c: kernel/x86/make_isrs.sh
	$< > $@

# Boot the kernel in qemu:
qemu-run: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio
# Same thing, but wait for gdb to attach:
qemu-gdb: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio -s -S

.PHONY: all qemu-run qemu-gdb
