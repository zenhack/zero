
TOOLCHAIN=arm-eabi

AS=$(TOOLCHAIN)-as
CC=$(TOOLCHAIN)-gcc
LD=$(TOOLCHAIN)-ld
OBJCOPY=$(TOOLCHAIN)-objcopy

OPT=-O2

CFLAGS := \
	-ffreestanding \
	-nostdlib \
	-fno-builtin \
	-Wall -Werror -Wextra \
	$(OPT)

OBJECTS := \
	boot.o \
	arch_main.o \
	act_led.o \
	sleep.o \
	mmio.o

kernel.out: kernel.elf
	$(OBJCOPY) $< -O binary $@
kernel.elf: $(OBJECTS)
	$(LD) -Tlink.ld -o $@ $(OBJECTS)
clean:
	rm -f *.o kernel.*
