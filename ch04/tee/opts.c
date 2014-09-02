/**
 * @file	opts.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "opts.h"

// for getopt
#include <unistd.h>
// for exit, NULL, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, stderr
#include <stdio.h>

/**
 * Prints usage.
 */
static void usage(void);

struct tee_opts read_opts(int argc, char * argv[])
{
	struct tee_opts opts = { .append = false, .filename = NULL };
	int ch;

	while ((ch = getopt(argc, argv, "a")) != -1) {
		if (ch == 'a') {
			opts.append = true;
		} else {
			// unknown option
			goto fail;
		}
	}

	// skip consumed option flags
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		// file not specified or too many trailing options
		goto fail;
	}

	opts.filename = argv[0];

	return opts;

fail:
	usage();
	exit(EXIT_FAILURE);
}

void usage(void)
{
	fprintf(stderr, "Usage: tee [-a] file\n");
}

