#include "newalloc.h"
// for assert
#include <assert.h>
// for intptr_t
#include <stdint.h>

#ifndef DEBUG
#define DEBUG 0
#endif

/**
 * Runs assertions to test the code.
 */
static void run_tests(void)
{
	na_node node;

	assert(sizeof(node) == 32);
	assert(NA_GET_DATA_PTR(node) == (void *)((intptr_t)&node + sizeof(na_node)));
}

int main(int argc, char * argv[])
{
#if DEBUG
	run_tests();
#endif

	void * mem = malloc(64);
	na_node * node = na_init_node(mem, 64);
	assert(node);
	assert(node->len == 32);
	assert(node->prev == NULL);
	assert(node->next == NULL);
	assert(NA_GET_DATA_PTR(node) == (void *)((intptr_t)&node + sizeof(na_node)));

	return 0;
}

