MLO: boot.elf
	arm-none-eabi-objcopy $< -O binary $@
boot.elf: boot.o link.ld
	arm-none-eabi-ld -T link.ld -o $@ boot.o
boot.o: boot.S
	arm-none-eabi-as -o $@ $<
