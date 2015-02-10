/**
 * @file       main.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

// for tail_read_opts, tail_opts
#include "opts.h"
// for tail_read_last_block, tail_free_block, tail_block
#include "block.h"
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, stderr, perror
#include <stdio.h>
// for open
#include <fcntl.h>
// for read, close
#include <unistd.h>
// for errno
#include <errno.h>
// for assert
#include <assert.h>

/** The size of the buffer for the file. */
// TODO: use a sufficiently big buffer
#define TAIL_FILE_BUF_SIZE 10

/**
 * Prints usage information.
 */
static void tail_usage(void)
{
    fprintf(stderr, "Usage: tail [ -n num] filename\n");
}

int main(int argc, const char * argv[])
{
    char buf[TAIL_FILE_BUF_SIZE];
    tail_opts opts;
    int fd = -1;
    int ret = EXIT_SUCCESS;
    ssize_t bytes;
    off_t offset;
    tail_block * block = NULL;

    if (tail_read_opts(argc, argv, &opts) != 0) {
        tail_usage();
        ret = EXIT_FAILURE;
        goto out;
    }

    if ((fd = open(opts.filename, O_RDONLY)) == -1) {
        perror("Opening file");
        ret = EXIT_FAILURE;
        goto out;
    }

    if ((block = tail_read_last_block(fd, 10)) == NULL) {
        perror("Reading block");
        ret = EXIT_FAILURE;
        goto out;
    }

    while (!block->is_first) {
        if ((block = tail_read_previous_block(fd, 10, block)) == NULL) {
            perror("Reading block");
            ret = EXIT_FAILURE;
            goto out;
        }
    }

    // print all blocks in order
    for (tail_block * cur = block; cur; cur = cur->next) {
        printf("%.*s", (int)cur->len, cur->data);
    }

    if (errno) {
        perror("Reading from file");
        ret = EXIT_FAILURE;
        goto out;
    }

out:
    if (fd != -1) {
        close(fd);
    }

    if (block) {
        tail_free_block(block);
    }

    return ret;
}

