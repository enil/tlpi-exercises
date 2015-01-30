/**
 * @file       opts.h
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#ifndef TAIL_OPTS_H
#define TAIL_OPTS_H

// for size_t
#include <stddef.h>

/**
 * Options for the tail command.
 */
typedef struct tail_opts {
    /** The number of lines to output. */
    size_t num_lines;

    /** The file to read from. */
    const char * filename;
} tail_opts;

/**
 * Reads command-line options for tail.
 */
extern int tail_read_opts(int argc, const char * argv[], tail_opts * opts);

#endif

