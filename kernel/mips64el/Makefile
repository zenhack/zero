
CC=mips64el-gcc
LD=mips64el-ld

$(TARG)_OFILES +=\
	$(TOP)/kernel/mips64el/arch_main.o\
	$(TOP)/kernel/mips64el/env.o\
	$(TOP)/kernel/mips64el/pmon2k_stdio.o\
	$(TOP)/kernel/mips64el/tlb_refill.o

CFLAGS += -G 0
LDFLAGS=-T$(TOP)/kernel/mips64el/link.ld -G 0
