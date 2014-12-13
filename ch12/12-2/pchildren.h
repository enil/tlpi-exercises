/**
 * @file	pchildren.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef PTREE_PCHILDREN_H
#define PTREE_PCHILDREN_H

// forward declaration
typedef struct proc_t proc_t;
typedef struct proc_list_t proc_list_t;

/**
 * A child node for a process information structure pointing to a child process.
 */
typedef struct proc_child_t {
    proc_t * proc;
    struct proc_child_t * next;
} proc_child_t;

/**
 * Iterates over each child process for a process.
 */
#define EACH_PROC_CHILD(parent, child) \
    for (proc_child_t * node = parent->first_child; node; node = node->next) \
        if (child = node->proc)

/**
 * Finds and adds the process children for every process.
 */
extern int populate_proc_children(proc_list_t * proc_list);

/**
 * Creates and adds a process child node its parent process.
 */
extern int add_proc_child(proc_t * parent_proc, proc_t * child_proc);

/**
 * Frees a child node structure.
 */
extern void free_child(proc_child_t * child);

#endif

