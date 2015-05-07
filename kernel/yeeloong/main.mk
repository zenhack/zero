CFLAGS += -G 0
LDFLAGS += -G 0

all: kernel.yeeloong.elf

ssrc += \
	kernel/yeeloong/boot.S\
	kernel/yeeloong/tlb_refill.S

csrc += \
	kernel/yeeloong/arch_main.c\
	kernel/yeeloong/env.c\
	kernel/yeeloong/pmon2k_stdio.c

.PHONY: all
