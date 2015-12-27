csrc += \
	kernel/port/data.c\
	kernel/port/heap.c\
	kernel/port/mmio.c\
	kernel/port/muxwriter.c\
	kernel/port/panic.c\
	kernel/port/stdio.c\
	kernel/port/string.c\
	kernel/port/sched.c

COMMON_CFLAGS += \
	-I $(srcdir)/kernel/port/include

include $(srcdir)/kernel/port/tests/main.mk
