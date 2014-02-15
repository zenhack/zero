include config.mak
# x86:

CC_x86 = cc -m32
OBJCOPY_x86 = objcopy

TARG_x86 = kernel.x86.elf

# arm:

## rpi:

CC_rpi = arm-none-eabi-gcc
OBJCOPY_rpi = arm-none-eabi-objcopy

TARG_rpi = kernel.rpi.bin

## panda:

CC_panda = arm-none-eabi-gcc
OBJCOPY_panda = arm-none-eabi-objcopy

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

CC = $(CC_$(ARCH))
# We need to link against libgcc for things like e.g. 64 bit division
# stubs. It's easier if we use gcc as the linker:
LD = $(CC)
OBJCOPY = $(OBJCOPY_$(ARCH))

BOOTSRC=kernel/$(ARCH)/boot.S
CSRC=$(wildcard kernel/$(ARCH)/*.c kernel/port/*.c)
SSRC=$(filter-out $(BOOTSRC), $(wildcard kernel/$(ARCH)/*.S))
OBJS=$(BOOTSRC:.S=.o) $(SSRC:.S=.o) $(CSRC:.c=.o)

TARG = $(TARG_$(ARCH))

all: $(TARG)

kernel.$(ARCH).elf: $(OBJS)
	$(LD) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS) -Tkernel/$(ARCH)/link.ld
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
