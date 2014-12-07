#ifndef PTREE_PROC_H
#define PTREE_PROC_H

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
    const char * name;
} proc_t;

/**
 * Reads process information from a proc file.
 */
extern proc_t * read_proc(FILE * file);

/**
 * Frees the memory used by a process information structure.
 */
extern void free_proc(proc_t * proc);

#endif

