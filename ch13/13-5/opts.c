/**
 * @file       opts.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2014
 */

#include "opts.h"

// for strtol
#include <stdlib.h>
// for LONG_MIN, LONG_MAX
#include <limits.h>
// for strcmp
#include <string.h>

/** The default number of lines to output. */
#define TAIL_DEFAULT_NUM_LINES 10

/**
 * Reads a size_t value.
 */
static int tail_read_size_t(const char * str, size_t * val_p);

int tail_read_opts(int argc, const char * argv[], tail_opts * opts)
{
    // TODO: use getopt

    const char ** next_arg = &argv[1];
    const char * filename = NULL;
    size_t num_lines = TAIL_DEFAULT_NUM_LINES;

    if (argc == 4 && strcmp(*next_arg++, "-n") == 0) {
        if (tail_read_size_t(*next_arg++, &num_lines) != 0) {
            // invalid number
            return -1;
        }
    } else if (argc != 2) {
        // invalid number of arguments
        return -1;
    }

    filename = *next_arg;

    opts->filename = filename;
    opts->num_lines = num_lines;

    // success
    return 0;
}

int tail_read_size_t(const char * str, size_t * val_p)
{
    char * end;

    int long_val = strtol(str, &end, 10);
    if (long_val == LONG_MIN || long_val == LONG_MAX || *end != '\0') {
        // not a valid number
        return -1;
    }

    *val_p = (size_t)long_val;
    return 0;
}

