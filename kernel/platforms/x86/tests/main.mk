
%.test: %.c
	cc -m32 $(COMMON_CFLAGS) -pthread -o $@ $<

tests := \
	kernel/platforms/x86/tests/lock_is_atomic.test\
	kernel/platforms/x86/tests/lock_checks_double_release.test

check: $(tests)
	for f in $(tests); do \
		./$$f ; \
	done


.PHONY: check