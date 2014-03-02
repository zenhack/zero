include config.mak

PLATFORM ?= x86

# x86:

CC_x86 = cc -m32
OBJCOPY_x86 = objcopy

TARG_x86 = kernel.x86.elf

# arm:

ARM_TOOL_PREFIX ?= arm-none-eabi-
ARM_TOOL_SUFFIX ?=

## rpi:

CC_rpi = $(ARM_TOOL_PREFIX)gcc$(ARM_TOOL_SUFFIX)
OBJCOPY_rpi = $(ARM_TOOL_PREFIX)objcopy$(ARM_TOOL_SUFFIX)

TARG_rpi = kernel.rpi.bin

## panda:

CC_panda = $(ARM_TOOL_PREFIX)gcc$(ARM_TOOL_SUFFIX)
OBJCOPY_panda = $(ARM_TOOL_PREFIX)objcopy$(ARM_TOOL_SUFFIX)

TARG_panda = MLO

# portable:

OPT=-O2
DEBUG=-g

CFLAGS +=\
	-ffreestanding\
	-nostdlib\
	-fno-builtin\
	$(DEBUG)\
	$(OPT)\
	-Wall -Werror -Wextra -Wno-unused-parameter\
	-I .

LIBS=-lgcc

CC = $(CC_$(PLATFORM))
# We need to link against libgcc for things like e.g. 64 bit division
# stubs. It's easier if we use gcc as the linker:
LD = $(CC)
OBJCOPY = $(OBJCOPY_$(PLATFORM))

BOOTSRC=kernel/$(PLATFORM)/boot.S
CSRC=$(wildcard kernel/$(PLATFORM)/*.c kernel/port/*.c)
SSRC=$(filter-out $(BOOTSRC), $(wildcard kernel/$(PLATFORM)/*.S))
OBJS=$(BOOTSRC:.S=.o) $(SSRC:.S=.o) $(CSRC:.c=.o)

TARG = $(TARG_$(PLATFORM))

all: $(TARG)

kernel.$(PLATFORM).elf: $(OBJS)
	$(LD) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS) -Tkernel/$(PLATFORM)/link.ld
clean:
	rm -f */*/*.o *.elf *.bin MLO
%.o: %.S
	$(CC) $(CFLAGS) -DASM_FILE -c -o $@ $<


# rpi:

kernel.rpi.bin: kernel.rpi.elf
	$(OBJCOPY) $< -O binary $@

# panda:

MLO: kernel.panda.elf
	$(OBJCOPY) $< -O binary $@
	$(PWD)/kernel/panda/scripts/patch-size
