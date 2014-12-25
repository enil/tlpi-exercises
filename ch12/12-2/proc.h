/**
 * @file	proc.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef PTREE_PROC_H
#define PTREE_PROC_H

// for proc_child_t
#include "pchildren.h"
// for FILE
#include <stdio.h>
// for pid_t
#include <sys/types.h>

/**
 * Information about a process.
 */
typedef struct proc_t {
    /** The PID of the process. */
    pid_t pid;

    /** The PID of the parent process. */
	pid_t ppid;

    /** The command name. */
    char * name;

    /** The first child process node. */
    proc_child_t * first_child;

    /** The last child process node. */
    proc_child_t * last_child;
} proc_t;

/**
 * Reads process information from a proc file.
 */
extern proc_t * read_proc(FILE * file);

/**
 * Frees a process information structure.
 */
extern void free_proc(proc_t * proc);

#endif

