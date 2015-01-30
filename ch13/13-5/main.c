/**
 * @file       main.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

// for tail_read_opts, tail_opts
#include "opts.h"
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for printf, fprintf, stderr
#include <stdio.h>

/**
 * Prints usage information.
 */
static void tail_usage(void)
{
    fprintf(stderr, "Usage: tail [ -n num] filename\n");
}

int main(int argc, const char * argv[])
{
    tail_opts opts;

    if (tail_read_opts(argc, argv, &opts) != 0) {
        tail_usage();
        return EXIT_FAILURE;
    }

    printf("n=%lld, filename=%s\n", (long long)opts.num_lines, opts.filename);

    return EXIT_SUCCESS;
}

