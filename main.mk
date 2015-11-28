VPATH := $(srcdir)

COMMON_CFLAGS :=
METAL_CFLAGS :=

include $(srcdir)/kernel/$(platform)/main.mk
include $(srcdir)/kernel/port/main.mk

DEBUG ?= -g
OPT ?= -O2

COMMON_CFLAGS += \
	-std=c11 \
	-Wall -Werror -Wextra -Wno-unused-parameter \
	$(DEBUG) \
	$(OPT)

METAL_CFLAGS += \
	-nostdlib \
	-ffreestanding

CFLAGS := $(COMMON_CFLAGS) $(METAL_CFLAGS)

LIBS = -lgcc

linker_script = $(srcdir)/kernel/$(platform)/link.ld

objects := $(ssrc:.S=.o) $(csrc:.c=.o)

depfiles := $(objects:.o=.dmk)

kernel.$(platform).elf: $(objects) $(linker_script)
	@echo LINK $@
	@$(LD) -o $@ $(objects) $(CFLAGS) $(LDFLAGS) $(LIBS) -T $(linker_script)

clean:
	@echo CLEAN
	@rm -f \
		$(shell find * -name '*.o') \
		kernel.*.elf \
		$(depfiles) \
		$(cleanfiles)

%.o: %.S
	@echo AS $<
	@$(CC) -MMD -MF $@.dmk $(CFLAGS) -DASM_FILE -c -o $@ $<
%.o: %.c
	@echo CC $<
	@$(CC) -MMD -MF $@.dmk $(CFLAGS) -c -o $@ $<

-include $(depfiles)
