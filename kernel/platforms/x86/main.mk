all: kernel.x86.elf

ssrc += \
	kernel/platforms/x86/boot.S\
	kernel/platforms/x86/cpuid.S\
	kernel/platforms/x86/gdt_load.S\
	kernel/platforms/x86/isr_stub.S\
	kernel/platforms/x86/lidt.S\
	kernel/platforms/x86/paging_asm.S

csrc += \
	kernel/platforms/x86/8259pic.c\
	kernel/platforms/x86/acpi.c\
	kernel/platforms/x86/apic.c\
	kernel/platforms/x86/apic_setup.c\
	kernel/platforms/x86/apic_timer.c\
	kernel/platforms/x86/apic_timer_setup.c\
	kernel/platforms/x86/ap_main.c\
	kernel/platforms/x86/arch_main.c\
	kernel/platforms/x86/gdt.c\
	kernel/platforms/x86/idt.c\
	kernel/platforms/x86/stdio_setup.c\
	kernel/platforms/x86/paging.c\
	kernel/platforms/x86/panic.c\
	kernel/platforms/x86/pit.c\
	kernel/platforms/x86/serial.c\
	kernel/platforms/x86/text_console.c\
	kernel/platforms/x86/thread.c\
	kernel/platforms/x86/mp_setup.c\
	\
	kernel/platforms/x86/isr_gen.c

cleanfiles += \
	kernel/platforms/x86/isr_gen.c

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/platforms/x86/include

HOST_CC := cc -m32

QEMU_FLAGS += \
	-smp 2
kernel/platforms/x86/isr_gen.c: kernel/platforms/x86/make_isrs.sh
	$< > $@

boot.iso: iso_build_dir/boot/kernel.x86.elf iso_build_dir/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso_build_dir
iso_build_dir/boot/kernel.x86.elf iso_build_dir/boot/grub/grub.cfg: iso_build_dir/boot/grub
iso_build_dir/boot/grub:
	mkdir -p $@
iso_build_dir/boot/grub/grub.cfg: kernel/platforms/x86/grub.cfg
	cp $< $@
iso_build_dir/boot/kernel.x86.elf: kernel.x86.elf
	cp $< $@

bochsrc: kernel/platforms/x86/bochsrc
	cp $< $@
bochs-run: boot.iso bochsrc
	bochs

# Boot the kernel in qemu:
qemu-run: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio $(QEMU_FLAGS)
# Same thing, but wait for gdb to attach:
qemu-gdb: all
	qemu-system-i386 -kernel $(objdir)/kernel.x86.elf -serial stdio -s -S $(QEMU_FLAGS)

include $(srcdir)/kernel/platforms/x86/tests/main.mk

.PHONY: all qemu-run qemu-gdb bochs-run
