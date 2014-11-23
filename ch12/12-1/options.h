/**
 * @file	options.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef OPTIONS_H
#define OPTIONS_H

// for uid_t
#include <sys/types.h>

/**
 * Options for the program.
 */
typedef struct proc_options_t {
	/** The PID of the process owner. */
	uid_t uid;
} proc_options_t;

/**
 * Parses options from the command line.
 */
extern int read_options(proc_options_t * options, int argc, const char * argv[]);

#endif

