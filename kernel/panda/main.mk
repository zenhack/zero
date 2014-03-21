all: $(objdir)/MLO

$(objdir)/MLO: $(objdir)/kernel.panda.elf
	$(OBJCOPY) $< -O binary $@
	$(srcdir)/kernel/panda/scripts/patch-size

cleanfiles = $(objdir)/MLO
