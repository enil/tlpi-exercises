 /**
 * @file	opts.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "opts.h"

// for fprintf, stderr
#include <stdio.h>
// for strtol
#include <stdlib.h>
// for basename
#include <libgen.h>

/**
 * Prints usage.
 */
static void usage(char * path)
{
	char * name = basename(path);
	fprintf(stderr, "Usage: %s filename num_buffers\n", name);
}

int read_opts(int argc, char * argv[], struct sg_opts * options)
{
	char * end;

	if (argc != 3) {
		goto fail;

	}

	options->filename = argv[1];

	options->num_bufs = strtol(argv[2], &end, 10);
	if (*end != '\0') {
		// argument not a number
		goto fail;
	}

	return 0;

fail:
	usage(argv[0]);
	return -1;
}
