include config.mak
# x86:

CC_x86 = cc -m32
LD_x86 = ld -melf_i386
OBJCOPY_x86 = objcopy

TARG_x86 = kernel.x86.elf

# rpi:

CC_rpi = arm-none-eabi-gcc
LD_rpi = arm-none-eabi-ld
OBJCOPY_rpi = arm-none-eabi-objcopy

TARG_rpi = kernel.rpi.bin

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

CC = $(CC_$(ARCH))
LD = $(LD_$(ARCH))
OBJCOPY = $(OBJCOPY_$(ARCH))


CSRC=$(wildcard kernel/$(ARCH)/*.c kernel/port/*.c)
SSRC=$(wildcard kernel/$(ARCH)/*.S)
OBJS=$(SSRC:.S=.o) $(CSRC:.c=.o)

TARG = $(TARG_$(ARCH))

all: $(TARG)

kernel.$(ARCH).elf: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) -Tkernel/$(ARCH)/link.ld
clean:
	rm -f */*/*.o $(TARG)
%.o: %.S
	$(CC) -c -o $@ $(CFLAGS) -DASM_FILE $<


# rpi:

kernel.rpi.bin: kernel.rpi.elf
	$(OBJCOPY) $< -O binary $@
