/**
 * @file       main.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2015
 */

// for tail_read_lines
#include "tail.h"
// for tail_read_opts, tail_opts
#include "opts.h"
// for printf, fprintf, perror, stderr
#include <stdio.h>
// for calloc, free, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for open, O_RDONLY
#include <fcntl.h>
// for close
#include <unistd.h>

/** The size of the buffer for the file. */
#ifndef TAIL_FILE_BUF_SIZE
#   define TAIL_FILE_BUF_SIZE (1 << 10) // 1KiB
#endif

/**
 * Prints usage information.
 */
static void tail_usage(void)
{
    fprintf(stderr, "Usage: tail [ -n num] filename\n");
}

int main(int argc, const char * argv[])
{
    // command-line options
    tail_opts opts;
    // actual number of read lines
    int lines_read;
    // the exit code
    int ret = EXIT_FAILURE;
    // the input file
    int fd = -1;
    // line buffers
    char ** lines = NULL;

    if (tail_read_opts(argc, argv, &opts) != 0) {
        perror("tail_read_opts");
        goto out;
    }

    if ((fd = open(opts.filename, O_RDONLY)) == -1) {
        perror("open");
        goto out;
    }

    // allocate line buffers
    if ((lines = calloc(opts.num_lines, sizeof(char *))) == NULL) {
        perror("calloc");
        goto out;
    }

    if ((lines_read = tail_read_lines(fd, TAIL_FILE_BUF_SIZE, lines, opts.num_lines)) < 0) {
        perror("tail_read_lines");
        goto out;
    }

    for (int index = opts.num_lines - lines_read; index < opts.num_lines; ++index) {
        char * line = lines[index];
        printf("%s", line);
        free(line);
    }

    ret = EXIT_SUCCESS;

out:
    if (lines) {
        free(lines);
    }
    if (fd != -1) {
        close(fd);
    }

    return ret;
}

