
the_test := kernel/x86/tests/lock.test

$(the_test): $(the_test:.test=.c)
	cc -m32 $(COMMON_CFLAGS) -pthread -o $@ $<

check: $(the_test)
	./$(the_test)

.PHONY: check
