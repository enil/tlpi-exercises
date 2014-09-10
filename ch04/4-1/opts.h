/**
 * @file	opts.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef TEE_OPTS_H
#define TEE_OPTS_H

// for bool
#include <stdbool.h>

struct tee_opts {
	bool append;
	char * filename;
};

/**
 * Reads options.
 */
extern struct tee_opts read_opts(int argc, char * argv[]);

#endif

