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

CC_x86 = cc -m32
LD_x86 = ld -melf_i386

CC = $(CC_$(ARCH))
LD = $(LD_$(ARCH))

TARG=kimage.out

CSRC=$(wildcard kernel/$(ARCH)/*.c kernel/port/*.c)
SSRC=$(wildcard kernel/$(ARCH)/*.S)
OBJS=$(SSRC:.S=.o) $(CSRC:.c=.o)

$(TARG): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) -Tkernel/$(ARCH)/link.ld
clean:
	rm -f */*/*.o $(TARG)
%.o: %.S
	$(CC) -c -o $@ $(CFLAGS) -DASM_FILE $<
