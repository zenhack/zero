all: kernel.rpi.bin

kernel.rpi.bin: kernel.rpi.elf
	@$(OBJCOPY) $< -O binary $@

ssrc += \
	kernel/rpi/boot.S\
	kernel/rpi/sleep.S


csrc += \
	kernel/rpi/act_led.c\
	kernel/rpi/arch_main.c\
	kernel/rpi/gpio.c\
	kernel/rpi/morse.c\
	kernel/rpi/morse_led.c\
	kernel/rpi/uart.c

cleanfiles = kernel.rpi.bin

.PHONY: all
