#ifndef NA_ALLOC_H
#define NA_ALLOC_H

// for size_t
#include <stdlib.h>
// for bool
#include <stdbool.h>
// for intptr_t
#include <stdint.h>

/**
 * A free or allocated memory node.
 */
typedef struct __attribute__((__aligned__(16))) na_node {
	/** The data capacity of the node. */
	size_t len;

	/** Whether the node is allocated. */
	bool used;

	/** The previous node in the list. */
	struct na_node * prev;

	/** The next node in the list. */
	struct na_node * next;
} na_node;

/**
 * Aligns a pointer "rounding" up.
 *
 * Alignment should be a power of 2.
 */
#define NA_ALIGN_PTR(ptr, type, align) ((type)(((intptr_t)ptr + ((1 << align) - 1)) & (~0 << align)))

/**
 * Aligns a size "rounding" up.
 */
#define NA_ALIGN_SIZE_T(size, align) ((size_t)((size + ((1 << align) - 1)) & (~0 << align)))

/**
 * Checks alignment on a pointer.
 *
 * Alignment should be a power of 2.
 */
#define NA_CHECK_ALIGN_PTR(ptr, align) (!((intptr_t)ptr & ~(~0 << align)))

/**
 * Gets the data for an allocated memory node.
 */
#define NA_GET_DATA_PTR(node) ((void *)node + sizeof(na_node))

/**
 * Gets the allocated memory node for its data pointer.
 */
#define NA_GET_NODE_PTR(data) ((void *)data - sizeof(na_node))

/**
 * Gets the total size of a memory node.
 */
#define NA_GET_NODE_SIZE(node) (sizeof(na_node) + node->len)

/**
 * Gets the end of a node.
 */
#define NA_GET_NODE_END(node) ((void *)node + NA_GET_NODE_SIZE(node))

/**
 * Allocates memory on the heap.
 */
extern void * na_malloc(size_t size);

/**
 * Frees memory allocated on the heap.
 */
extern void na_free(void * data);

/**
 * Reserves memory and creates a memory node.
 */
extern na_node * na_reserve_node_mem(size_t size);

/**
 * Initializes a memory node.
 *
 * size is the total available size for the node and its data.
 */
extern na_node * na_init_node(void * position, size_t size);

/**
 * Reserves memory for dynamic allocation.
 */
extern void * na_reserve_mem(size_t size);

/**
 * Prints information about the memory nodes.
 */
extern void na_print_nodes(void);

/**
 * Prints list of used memory nodes.
 */
extern void na_list_nodes(void);


#endif

