#ifndef NA_ALLOC_H
#define NA_ALLOC_H

// for size_t
#include <stdlib.h>

/**
 * Allocates memory on the heap.
 */
extern void * na_malloc(size_t len);

/**
 * Frees memory allocated on the heap.
 */
extern void na_free(void * data);

/**
 * Prints information about the memory nodes.
 */
extern void na_print_nodes(void);

/**
 * Prints list of used memory nodes.
 */
extern void na_list_nodes(void);

#endif

