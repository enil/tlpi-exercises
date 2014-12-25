/**
 * @file	main.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for match_all_proc_files
#include "psfile.h"
// for fprintf, stderr
#include <stdio.h>
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

typedef struct pstree_options {
    const char * filename;
} pstree_options;

/**
 * Reads options for the program.
 */
static int read_options(pstree_options * options, int argc, const char * argv[])
{
    if (argc != 2) {
        // wrong number of arguments, needs a filename
        return -1;
    }

    options->filename = argv[1];
    return 0;
}

/**
 * Prints usage information for the program.
 */
static void print_usage(void)
{
    fprintf(stderr, "Usage: psfile filename\n");
}

int main(int argc, const char * argv[])
{
    pstree_options options;

    if (read_options(&options, argc, argv) != 0) {
        print_usage();
        return EXIT_FAILURE;
    }

    if (match_all_proc_files(options.filename) != 0) {
        perror("Matching proc files");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

