TOP=$(PWD)/../

OPT=-O2

CFLAGS +=\
	-ffreestanding\
	-nostdlib\
	-fno-builtin\
	-g\
	$(OPT)\
	-Wall -Werror -Wextra -Wno-unused-parameter\
	-I $(TOP)

TARG=kimage.out
$(TARG)_OFILES=

include $(TOP)/kernel/$(ARCH)/Makefile
include $(TOP)/kernel/port/Makefile

$(TARG): $($(TARG)_OFILES)
	$(LD) -o $@ $($(TARG)_OFILES) $(LDFLAGS) 
clean:
	rm -f */*.o $(TARG)

%.o: %.S
	$(CC) -c -o $@ $(CFLAGS) -DASM_FILE $<
