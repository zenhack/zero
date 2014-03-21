include $(srcdir)/kernel/$(platform)/main.mk

DEBUG ?= -g
OPT ?= -O2

CFLAGS += \
	-ffreestanding \
	-nostdlib \
	-Wall -Werror -Wextra -Wno-unused-parameter \
	-I $(srcdir) \
	$(DEBUG) \
	$(OPT)

LIBS = -lgcc

csrc = \
	$(shell find $(srcdir)/kernel/$(platform) -name '*.c') \
	$(shell find $(srcdir)/kernel/port -name '*.c')
ssrc = \
	$(srcdir)/kernel/$(platform)/boot.S \
	$(shell find $(srcdir)/kernel/$(platform) -name '*.S' | grep -v boot.S)

linker_script = $(srcdir)/kernel/$(platform)/link.ld

objects = \
	$(patsubst $(srcdir)/%, $(objdir)/%, $(ssrc:.S=.o) $(csrc:.c=.o))

sdirs = $(shell find $(srcdir)/ -type d | grep -v '\.hg')
odirs = $(patsubst $(srcdir)/%, $(objdir)/%, $(sdirs))

$(objdir)/kernel.$(platform).elf: $(objects) $(linker_script)
	$(LD) -o $@ $(objects) $(CFLAGS) $(LDFLAGS) $(LIBS) -T $(linker_script)

clean:
	rm -f \
		$(shell find $(objdir) -name '*.o') \
		$(objdir)/kernel.*.elf \
		$(cleanfiles)

$(objdir)/%.o: $(srcdir)/%.S | $(odirs)
	$(CC) $(CFLAGS) -DASM_FILE -c -o $@ $<
$(objdir)/%.o: $(srcdir)/%.c | $(odirs)
	$(CC) $(CFLAGS) -c -o $@ $<
$(odirs):
	[ -d $@ ] || mkdir -p $@
