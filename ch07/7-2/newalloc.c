#include "newalloc.h"

// for abort
#include <stdlib.h>
// for sbrk
#include <unistd.h>
// for ptrdiff_t
#include <stddef.h>
// for assert
#include <assert.h>
// for true, false
#include <stdbool.h>
// for printf
#include <stdio.h>

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

/**
 * The default size of memory to reserve for dynamic allocation
 */
#define NA_DEFAULT_RESERVE_MEM_SIZE (1 << 12)

/**
 * The first memory node.
 */
static na_node * na_first_node = NULL;

/**
 * Adds a new node to the end of the node list.
 */
static na_node * na_append_node(na_node * node);

/**
 * Splits an unused node and returns the first half.
 */
static na_node * na_split_node(na_node * node, size_t size);

/**
 * Joins an unused node with its successor.
 */
static na_node * na_join_node(na_node * node);

/**
 * Finds the first node to fit the required data size.
 */
static na_node * na_find_fitting_node(size_t size);

/**
 * Finds the last node.
 */
static na_node * na_get_last_node(void);

void * na_malloc(size_t size)
{
	// find a node that fits
	na_node * fitting_node = na_find_fitting_node(size);
	// reserve more space if no node was found
	if (fitting_node == NULL) {
		na_node * reserved_node = na_reserve_node_mem(NA_DEFAULT_RESERVE_MEM_SIZE);
		if (reserved_node == NULL) {
			// failed to reserve node
			return NULL;
		}
		// FIXME: won't work if size > NA_DEFAULT_RESERVE_MEM_SIZE
		fitting_node = na_append_node(reserved_node);
	}

	if (NA_GET_NODE_SIZE(fitting_node) >= (NA_MIN_NODE_SIZE << 1)) {
		// the fitting node is big enough to split
		fitting_node = na_split_node(fitting_node, size);
		if (fitting_node == NULL) {
			// failed to split node
			return NULL;
		}
	}

	// mark as used
	fitting_node->used = true;

	return NA_GET_DATA_PTR(fitting_node);
}

void na_free(void * data)
{
	// get the node
	na_node * node = NA_GET_NODE_PTR(data);

	if (!node->used) {
		// attempted to free non-allocated memory
		abort();
	}

	// mark as unused
	node->used = false;

	if (node->next && !node->next->used) {
		// can join with the succeeding node
		node = na_join_node(node);
	}

	if (node->prev && !node->prev->used) {
		// can join with preceding node
		node = na_join_node(node->prev);
	}
}

na_node * na_reserve_node_mem(size_t size)
{
	// check the size
	if (size < NA_MIN_NODE_SIZE) {
		// TODO: error code
		return NULL;
	}

	// reserve memory for the node
	void * start = na_reserve_mem(size);
	if (start == NULL) {
		return NULL;
	}

	// WARNING: reserved memory will be wasted if node initalization fails
	na_node * node = na_init_node(start, size);
	if (node == NULL) {
		return NULL;
	}

	return node;
}

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
	ptrdiff_t diff = NA_ALIGN_PTR(current, void *, NA_NODE_ALIGNMENT) - current;

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

void na_print_nodes(void)
{
	// header
	printf("%-4s  %-10s  %-10s  %-10s  %-6s  %-6s  %-3s\n", "idx", "node", "end",  "data", "size", "length", "used");

	int index = 0;

	for (na_node * node = na_first_node; node; node = node->next) {
		printf("%-4d  0x%08lx  0x%08lx  0x%08lx  %-6lu  %-6lu  %-3s\n",
				index++,
				(intptr_t)node,
				(intptr_t)NA_GET_NODE_END(node),
				(intptr_t)NA_GET_DATA_PTR(node),
				(unsigned long)NA_GET_NODE_SIZE(node),
				(unsigned long)node->len,
				(node->used ? "yes" : "no"));
	}
}

void na_list_nodes(void)
{
	// header
	printf("%-10s  %-6s\n", "data", "length");

	for (na_node * node = na_first_node; node; node = node->next) {
		if (node->used) {
			printf("0x%08lx  %-6lu\n", (intptr_t)NA_GET_DATA_PTR(node), (unsigned long)node->len);
		}
	}
}

na_node * na_split_node(na_node * node, size_t size)
{
	assert(!node->used);

	// size of the old node
	size_t old_size = node->len + sizeof(na_node);
	// size of the left and right nodes
	size_t left_size = NA_ALIGN_SIZE_T(size + sizeof(na_node), NA_NODE_ALIGNMENT);
	size_t right_size = old_size - left_size;

	// check that the sizes is adequate
	if (left_size < NA_MIN_NODE_SIZE || right_size < NA_MIN_NODE_SIZE) {
		return NULL;
	}

	// save reference to the next node
	na_node * next = node->next;

	// initalize right block
	na_node * right = na_init_node((void *)node + left_size, right_size);
	if (right == NULL) {
		return NULL;
	}

	// resize left block
	na_node * left = node;
	left->len = left_size - sizeof(na_node);
	left->next = right;

	// insert right between left and its successor
	right->prev = left;
	right->next = next;

	return left;
}

na_node * na_join_node(na_node * node)
{
	assert(node->next);
	assert(!node->used && !node->next->used);
	assert(NA_GET_NODE_END(node) == node->next);

	// the node to join with
	na_node * right = node->next;
	na_node * next = right->next;

	// size of the node to join with
	size_t right_size = NA_GET_NODE_SIZE(node->next);

	// expand the node
	node->len += right_size;
	node->next = next;

	return node;
}

na_node * na_append_node(na_node * node)
{
	na_node * last_node = na_get_last_node();

	if (last_node) {
		last_node->next = node;
	} else {
		na_first_node = node;
	}
	return node;
}

na_node * na_find_fitting_node(size_t size)
{
	for (na_node * node = na_first_node; node; node = node->next) {
		if (!node->used && node->len >= size) {
			return node;
		}
	}

	// none found
	return NULL;
}

na_node * na_get_last_node(void)
{
	na_node * node = na_first_node;
	// get the first node not having a successor
	while (node && node->next) {
		node = node->next;
	}
	return node;
}

