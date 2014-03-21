all: $(objdir)/kernel.rpi.bin

$(objdir)/kernel.rpi.bin: $(objdir)/kernel.rpi.elf
	$(OBJCOPY) $< -O binary $@

cleanfiles = $(objdir)/kernel.rpi.bin
