
all: kernel.rpi.bin

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/platforms/rpi/include \
	-march=armv6

kernel.rpi.bin: kernel.rpi.elf
	@$(OBJCOPY) $< -O binary $@

ssrc += \
	kernel/platforms/rpi/boot.S\
	kernel/platforms/rpi/sleep.S


csrc += \
	kernel/platforms/rpi/act_led.c\
	kernel/platforms/rpi/arch_main.c\
	kernel/platforms/rpi/gpio.c\
	kernel/platforms/rpi/morse.c\
	kernel/platforms/rpi/morse_led.c\
	kernel/platforms/rpi/uart.c

cleanfiles = kernel.rpi.bin

.PHONY: all
