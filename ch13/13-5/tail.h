/**
 * @file       tail.h
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2015
 */

#ifndef TAIL_TAIL_H
#define TAIL_TAIL_H

// for size_t
#include <stddef.h>

/**
 * Reads lines from the end of a file.
 */
extern int tail_read_lines(int fd, size_t buf_size, char ** lines, int num_lines);

#endif

