#include <kernel/arch/lock.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

static volatile int counter;
static mutex_t lock;

void *do_thread(void *arg) {
	int sign = (int)arg;

	for(int i = 0; i < 100000; i++) {
		wait_acquire(&lock);
		counter += sign;
		release(&lock);
	}

	return NULL;
}

void panic(char *s) {
	fprintf(stderr, "%s\n", s);
	exit(1);
}

int main(void) {
	pthread_t incr, decr;

	pthread_create(&incr, NULL, do_thread, (void *)1);
	pthread_create(&decr, NULL, do_thread, (void *)-1);

	pthread_join(incr, NULL);
	pthread_join(decr, NULL);

	if(counter != 0) {
		panic("Counter is non-zero!");
	}
	return 0;
}
