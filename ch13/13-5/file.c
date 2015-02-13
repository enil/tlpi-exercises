/**
 * @file       file.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#include "file.h"

// for tail_free_block, tail_block
#include "block.h"
// for open, O_RDONLY
#include <fcntl.h>
// for close
#include <unistd.h>
// for malloc, free
#include <stdlib.h>
// for memset
#include <string.h>

/**
 * Allocates a file.
 */
static tail_file * tail_alloc_file(int fd, size_t block_len);

tail_file * tail_create_file(int fd, size_t block_len)
{
    tail_file * file = NULL;

    if ((file = tail_alloc_file(fd, block_len)) == NULL) {
        goto fail;
    }

    return file;

fail:
    if (file) {
        tail_free_file(file);
    }
    return NULL;
}

tail_file * tail_open_file(const char * filename, size_t block_len)
{
    int fd = -1;
    tail_file * file = NULL;

    if ((fd = open(filename, O_RDONLY)) == -1) {
        goto fail;
    }

    if ((file = tail_create_file(fd, block_len)) == NULL) {
        goto fail;
    }

    return file;

fail:
    if (fd != -1) {
        close(fd);
    }
    return NULL;
}

void tail_free_file(tail_file * file)
{
    if (file->first) {
        tail_block * next_block;
        for (tail_block * block = file->first; block; block = next_block) {
            next_block = block->next;
            tail_free_block(block);
        }
    }

    free(file);
}

void tail_close_file(tail_file * file)
{
    close(file->fd);
    tail_free_file(file);
}

tail_file * tail_alloc_file(int fd, size_t block_len)
{
    tail_file * file = NULL;

    if ((file = (tail_file *)malloc(sizeof(tail_file))) == NULL) {
        return NULL;
    }

    // clear the file structure
    memset(file, 0, sizeof(tail_file));

    file->fd = fd;
    file->block_len = block_len;

    return file;
}

