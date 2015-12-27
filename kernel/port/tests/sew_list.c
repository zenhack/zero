#include <assert.h>
#include <kernel/port/data.h>
/* XXX: Our build system still doesn't give us a good way for getting the object
 * file linked into a test, so we're doing this awful thing for now. Need to
 * revamp the build system soon. */
#include "../data.c"

#define BUFSIZE 8192

typedef struct SomeType SomeType;

struct SomeType {
	SomeType *next;
	size_t x;
	size_t y;
};

static uint8_t buffer[BUFSIZE];

int main(void) {
	SomeType *l = (SomeType *)sew_list(buffer, BUFSIZE, sizeof(SomeType));
	size_t num_items = 0;
	SomeType *cur = l;
	while(cur) {
		cur->x = num_items;
		num_items++;
		cur = cur->next;
	}
	assert(num_items == BUFSIZE / sizeof(SomeType));
	cur = l;
	for(size_t i = 0; i < num_items; i++) {
		assert(cur->x == i);
		cur = cur->next;
	}
}
