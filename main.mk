VPATH := $(srcdir)
include $(srcdir)/kernel/$(platform)/main.mk
include $(srcdir)/kernel/port/main.mk

DEBUG ?= -g
OPT ?= -O2

COMMON_CFLAGS += \
	-std=c11 \
	-Wall -Werror -Wextra -Wno-unused-parameter \
	-I $(srcdir) \
	$(DEBUG) \
	$(OPT)

METAL_CFLAGS += \
	-nostdlib \
	-ffreestanding

CFLAGS := $(COMMON_CFLAGS) $(METAL_CFLAGS)

LIBS = -lgcc

linker_script = $(srcdir)/kernel/$(platform)/link.ld

objects := $(ssrc:.S=.o) $(csrc:.c=.o)

depfiles := $(objects:.o=.deps.mk)

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

%.o: %.S %.deps.mk
	@echo AS $<
	@$(CC) $(CFLAGS) -DASM_FILE -c -o $@ $<
%.o: %.c %.deps.mk
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.deps.mk: %.S
	@echo DEPS $<
	@$(CPP) -M -MQ $(@:.deps.mk=.o) $(CFLAGS) -DASM_FILE $< > $@
%.deps.mk: %.c
	@echo DEPS $<
	@$(CPP) -M -MQ $(@:.deps.mk=.o) $(CFLAGS)  $< > $@

include $(depfiles)
