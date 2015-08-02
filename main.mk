VPATH := $(srcdir)
include $(srcdir)/kernel/$(platform)/main.mk
include $(srcdir)/kernel/port/main.mk

DEBUG ?= -g
OPT ?= -O2

CFLAGS += \
	-std=c11 \
	-ffreestanding \
	-nostdlib \
	-Wall -Werror -Wextra -Wno-unused-parameter \
	-I $(srcdir) \
	$(DEBUG) \
	$(OPT)

LIBS = -lgcc

linker_script = $(srcdir)/kernel/$(platform)/link.ld

objects = $(ssrc:.S=.o) $(csrc:.c=.o)

kernel.$(platform).elf: $(objects) $(linker_script)
	$(LD) -o $@ $(objects) $(CFLAGS) $(LDFLAGS) $(LIBS) -T $(linker_script)

clean:
	rm -f \
		$(shell find * -name '*.o') \
		kernel.*.elf \
		$(cleanfiles)

%.o: %.S
	$(CC) $(CFLAGS) -DASM_FILE -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
