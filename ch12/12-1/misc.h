/**
 * @file	misc.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef MISC_H
#define MISC_H

// for pid_t, uid_t
#include <sys/types.h>
// for bool
#include <stdbool.h>

/**
 * Checks if a string only consists of decimal digits.
 */
extern bool is_numeric_string(const char * str);

/**
 * Reads a UID from a string.
 */
extern int read_uid(const char * str, uid_t * uid_p);

/**
 * Reads a PID from a string.
 */
extern int read_pid(const char * str, pid_t * pid);

#endif

