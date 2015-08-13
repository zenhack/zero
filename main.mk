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

objects := $(ssrc:.S=.o) $(csrc:.c=.o)

depfiles := $(objects:.o=.deps.mk)

kernel.$(platform).elf: $(objects) $(linker_script)
	$(LD) -o $@ $(objects) $(CFLAGS) $(LDFLAGS) $(LIBS) -T $(linker_script)

clean:
	rm -f \
		$(shell find * -name '*.o') \
		kernel.*.elf \
		$(depfiles) \
		$(cleanfiles)

%.o: %.S %.deps.mk
	$(CC) $(CFLAGS) -DASM_FILE -c -o $@ $<
%.o: %.c %.deps.mk
	$(CC) $(CFLAGS) -c -o $@ $<

%.deps.mk: %.S
	$(CPP) -M -MQ $(@:.deps.mk=.o) $(CFLAGS) -DASM_FILE $< > $@
%.deps.mk: %.c
	$(CPP) -M -MQ $(@:.deps.mk=.o) $(CFLAGS)  $< > $@

include $(depfiles)
