/**
 * @file       block.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#include "block.h"

// for malloc, free
#include <stdlib.h>
// for memset
#include <string.h>
// for lseek, SEEK_SET, SEEK_END
#include <unistd.h>
// for off_t, size_t
#include <sys/types.h>
// for true, false
#include <stdbool.h>

/**
 * Allocates an empty block.
 */
static tail_block * tail_alloc_block(int fd);

/**
 * Reads to the buffer of a block.
 */
static int tail_read_buffer(int fd, tail_block * block);

tail_block * tail_read_last_block(int fd, size_t len)
{
    tail_block * block = NULL;
    off_t end;
    size_t buf_len;

    if ((block = tail_alloc_block(fd)) == NULL) {
        goto fail;
    }

    // find the end
    if ((end = lseek(fd, 0, SEEK_END)) == -1) {
        goto fail;
    }

    block->is_last = true;

    // FIXME: what to return if file is empty?
    if (len >= end) {
        // shrink block length if the file size is less than the buffer size
        len = (size_t)end;
        block->is_first = true;
    }
    block->len = len;

    // position the file offset to the start of the block
    if ((block->start = lseek(fd, -len, SEEK_END)) == -1) {
        goto fail;
    }

    // allocate the buffer
    if ((block->data = (char *)malloc(len)) == NULL) {
        goto fail;
    }

    if (tail_read_buffer(fd, block) != 0) {
        goto fail;
    }

    return block;

fail:
    if (block) {
        tail_free_block(block);
    }
    return NULL;
}

tail_block * tail_read_previous_block(int fd, size_t len, tail_block * next)
{
    tail_block * block = NULL;
    off_t end;

    if ((block = tail_alloc_block(fd)) == NULL) {
        goto fail;
    }

    end = next->start;

    if (len >= end) {
        // shrink block length if the remaining length is less than the buffer size
        len = (size_t)end;
        block->is_first = true;
    }
    block->len = len;

    // position the file offset to the start of the block
    if ((block->start = lseek(fd, end-len, SEEK_SET)) == -1) {
        goto fail;
    }

    // allocate the buffer
    if ((block->data = (char *)malloc(len)) == NULL) {
        goto fail;
    }

    if (tail_read_buffer(fd, block) != 0) {
        goto fail;
    }

    // connect with the next block
    next->prev = block;
    block->next = next;

    return block;

fail:
    if (block) {
        tail_free_block(block);
    }
    return NULL;
}

void tail_free_block(tail_block * block)
{
    if (block->data) {
        free(block->data);
    }
    free(block);
}

tail_block * tail_alloc_block(int fd)
{
    tail_block * block = NULL;

    if ((block = (tail_block *)malloc(sizeof(tail_block))) == NULL) {
        return NULL;
    }

    // clear the block structure
    memset(block, 0, sizeof(tail_block));

    block->fd = fd;

    return block;
}

int tail_read_buffer(int fd, tail_block * block)
{
    // bytes remaining to be read
    size_t remaining = block->len;
    // the position in the buffer
    char * buf = block->data;
    // number of bytes read
    size_t n;

    // read until the entire buffer is filled
    while (remaining > 0 && (n = read(fd, buf, remaining)) > 0) {
        remaining -= n;
        buf += n;
    }

    return n >= 0 && remaining == 0 ? 0 : -1;
}

