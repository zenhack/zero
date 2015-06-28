csrc += \
	kernel/port/heap.c\
	kernel/port/mmio.c\
	kernel/port/muxwriter.c\
	kernel/port/panic.c\
	kernel/port/stdio.c\
	kernel/port/string.c

tests += \
	kernel/port/heap.test

cleanfiles += $(tests)

%.test: %.test_o
	cc -o $@ $<
%.test_o: test_%.c
	cc -I $(srcdir) -c -o $@ $<
check: $(tests)
	for t in $(tests); do \
		./$$t ; \
	done
