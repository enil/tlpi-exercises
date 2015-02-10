/**
 * @file       block.h
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#ifndef TAIL_BLOCK_H
#define TAIL_BLOCK_H

// for off_t, size_t
#include <sys/types.h>
// for bool
#include <stdbool.h>

/**
 * A block read from a file.
 */
typedef struct tail_block {
    /** The file descriptor the the file the block is read from. */
    int fd;

    /** The start offset of the block. */
    off_t start;

    /** The length of the block. */
    size_t len;

    /** The data read from the file. */
    char * data;

    /** The previous block (NULL if not read). */
    struct tail_block * prev;

    /** The next block (NULL if not read). */
    struct tail_block * next;

    /** Whether the block is the first. */
    bool is_first;

    /** Whether the block is the last. */
    bool is_last;
} tail_block;

/**
 * Reads the last block in a file.
 */
extern tail_block * tail_read_last_block(int fd, size_t len);

/**
 * Reads the block before another block in a file.
 */
extern tail_block * tail_read_previous_block(int fd, size_t len, tail_block * prev);

/**
 * Frees a block.
 */
extern void tail_free_block(tail_block * block);

#endif

