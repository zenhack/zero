
include $(srcdir)/kernel/arch/arm/main.mk

all: MLO

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/platforms/panda/include \
	-march=armv6

MLO: kernel.panda.elf
	$(OBJCOPY) $< -O binary $@
	$(srcdir)/kernel/platforms/panda/scripts/patch-size

ssrc += \
	$(srcdir)/kernel/platforms/panda/boot.S

csrc += \
	$(srcdir)/kernel/platforms/panda/arch_main.c\
	$(srcdir)/kernel/platforms/panda/uart.c

cleanfiles = MLO
