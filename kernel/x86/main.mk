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
	kernel/x86/apic_setup.c\
	kernel/x86/apic_timer.c\
	kernel/x86/apic_timer_setup.c\
	kernel/x86/arch_main.c\
	kernel/x86/gdt.c\
	kernel/x86/idt.c\
	kernel/x86/stdio_setup.c\
	kernel/x86/paging.c\
	kernel/x86/panic.c\
	kernel/x86/pit.c\
	kernel/x86/serial.c\
	kernel/x86/text_console.c\
	kernel/x86/thread.c\
	kernel/x86/mp_setup.c\
	\
	kernel/x86/isr_gen.c

cleanfiles += \
	kernel/x86/isr_gen.c

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/x86/include

kernel/x86/isr_gen.c: kernel/x86/make_isrs.sh
	$< > $@

boot.iso: iso_build_dir/boot/kernel.x86.elf iso_build_dir/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso_build_dir
iso_build_dir/boot/kernel.x86.elf iso_build_dir/boot/grub/grub.cfg: iso_build_dir/boot/grub
iso_build_dir/boot/grub:
	mkdir -p $@
iso_build_dir/boot/grub/grub.cfg: kernel/x86/grub.cfg
	cp $< $@
iso_build_dir/boot/kernel.x86.elf: kernel.x86.elf
	cp $< $@

bochsrc: kernel/x86/bochsrc
	cp $< $@
bochs-run: boot.iso bochsrc
	bochs

# Boot the kernel in qemu:
qemu-run: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio
# Same thing, but wait for gdb to attach:
qemu-gdb: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio -s -S

include $(srcdir)/kernel/x86/tests/main.mk

.PHONY: all qemu-run qemu-gdb bochs-run
