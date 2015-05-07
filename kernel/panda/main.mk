all: MLO

MLO: kernel.panda.elf
	$(OBJCOPY) $< -O binary $@
	$(srcdir)/kernel/panda/scripts/patch-size

ssrc += \
	kernel/panda/boot.S

csrc += \
	kernel/panda/arch_main.c\
	kernel/panda/uart.c

cleanfiles = MLO
