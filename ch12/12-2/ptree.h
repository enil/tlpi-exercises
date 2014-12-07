/**
 * @file	ptree.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef PTREE_PTREE_H
#define PTREE_PTREE_H

// for proc_t
#include "proc.h"
// for size_t
#include <stddef.h>
// for pid_t
#include <sys/types.h>

/** The max number of processes that can be stored in a proc_list_t. */
#define PTREE_PROC_MAX 32768

/**
 * A list for the processes running in the system.
 */
typedef struct proc_list_t {
    /** The max number of processes. */
	size_t size;

    /** The processes by PID. */
	proc_t * procs[PTREE_PROC_MAX];
} proc_list_t;

/**
 * Iterates over each process in a process list.
 */
#define EACH_PROC_LIST(list, proc) proc_t * proc = NULL; for (int pos = 0; proc = list->procs[pos], pos < list->size; ++pos) if (proc)

extern proc_list_t * create_proc_list(void);

/**
 * Creates a process list structure.
 */
extern proc_list_t * alloc_proc_list(void);

/**
 * Frees a process list structure.
 */
extern void free_proc_list(proc_list_t * proc_list);

/**
 * Loads a process from a process list.
 */
extern proc_t * get_proc(proc_list_t * proc_list, pid_t pid);

/**
 * Stores a process in a process list.
 */
extern void put_proc(proc_list_t * proc_list, proc_t * proc);

#endif

