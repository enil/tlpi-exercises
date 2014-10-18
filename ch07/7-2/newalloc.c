#include "newalloc.h"

// for sbrk
#include <unistd.h>
// for ptrdiff_t
#include <stddef.h>
// for assert
#include <assert.h>
// for false
#include <stdbool.h>

/**
 * Alignment of memory nodes.
 */
#define NA_NODE_ALIGNMENT 4 /* 1<<4 == 16 */

/**
 * The smallest size of the data of a memory node.
 */
#define NA_MIN_DATA_SIZE (1 << 4)

/**
 * The smallest required size of a node.
 */
#define NA_MIN_NODE_SIZE (sizeof(na_node) + NA_MIN_DATA_SIZE)

na_node * na_init_node(void * position, size_t size)
{
	// align the position
	void * start = NA_ALIGN_PTR(position, void *, NA_NODE_ALIGNMENT);
	size -= (start-position);

	// check the size
	if (size < NA_MIN_NODE_SIZE) {
		return NULL;
	}

	// initialize the node
	na_node * node = (na_node *)start;
	node->len = size-sizeof(na_node);
	node->used = false;
	node->prev = node->next = NULL;
	
	return node;
}

void * na_reserve_mem(size_t size)
{
	// check alignment
	// WARNING: not thread safe, another thread can program break meanwhile!
	void * current = sbrk(0);
	if ((intptr_t)current == -1) {
		goto fail;
	}
	ptrdiff_t diff = NA_ALIGN_PTR(current, typeof(current), NA_NODE_ALIGNMENT) - current;

	if (diff) {
		// align program break
		current = sbrk(diff);
		if ((intptr_t)current == -1) {
			goto fail;
		}
		assert(NA_CHECK_ALIGN_PTR(current, NA_NODE_ALIGNMENT) == 0);
	}

	// reserve memory
	void * start = sbrk(size);
	if ((intptr_t)start == -1) {
		goto fail;
	}

	return start;

fail:
	return NULL;
}

