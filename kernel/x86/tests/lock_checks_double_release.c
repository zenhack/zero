#include <kernel/arch/lock.h>
#include <assert.h>

/* release() should call panic() if (and only if) passed an unlocked mutex. */

static mutex_t lock;
static int did_panic = 0;

void panic(char *s) {
	did_panic = 1;
}

int main(void) {
	wait_acquire(&lock);
	release(&lock);
	assert(!did_panic);
	release(&lock);
	assert(did_panic);
	return 0;
}
