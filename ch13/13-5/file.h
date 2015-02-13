/**
 * @file       file.h
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#ifndef TAIL_FILE_H
#define TAIL_FILE_H

// for tail_block
#include "block.h"
// for size_t
#include <stddef.h>

/**
 * A file that can be read in reverse order.
 */
typedef struct tail_file {
    /** The file descriptor the the file. */
    int fd;

    /** The length of the blocks. */
    size_t block_len;

    /** The first read block (NULL if not read). */
    tail_block * first;

    /** The last read block (NULL if not read). */
    tail_block * last;
} tail_file;

/**
 * Creates a file from a file descriptor.
 */
extern tail_file * tail_create_file(int fd, size_t block_len);

/**
 * Frees a file.
 */
extern void tail_free_file(tail_file * file);

/**
 * Opens and creates a file.
 */
extern tail_file * tail_open_file(const char * filename, size_t block_len);

/**
 * Closes and frees a file.
 */
extern void tail_close_file(tail_file * file);

#endif

