
all: MLO

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/panda/include \
	-march=armv6

MLO: kernel.panda.elf
	$(OBJCOPY) $< -O binary $@
	$(srcdir)/kernel/panda/scripts/patch-size

ssrc += \
	$(srcdir)/kernel/panda/boot.S

csrc += \
	$(srcdir)/kernel/panda/arch_main.c\
	$(srcdir)/kernel/panda/uart.c

cleanfiles = MLO
