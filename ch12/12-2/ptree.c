/**
 * @file	ptree.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "ptree.h"

// for create_proc_list, get_proc, free_proc_list
#include "plist.h"
// for populate_proc_children, proc_child_t
#include "pchildren.h"
// for proc_t
#include "proc.h"
// for fprintf, FILE
#include <stdio.h>
// for strlen
#include <string.h>

/** The PID of the init process. */
#define INIT_PROC_PID 1

/** The number of spaces for every indent level in the tree. */
#define TREE_INDENT_SPACES 2

/**
 * Writes a process and its children.
 */
static int write_proc(proc_t * proc, FILE * file, int level);

proc_tree_t * create_proc_tree(void)
{
    proc_tree_t * proc_tree = NULL;

    // create a list with all process
    if ((proc_tree = create_proc_list()) == NULL) {
        goto fail;
    }

    // connect the child processes with their parents
    if (populate_proc_children(proc_tree) != 0) {
        goto fail;
    }

    return proc_tree;

fail:
    if (proc_tree != NULL) {
        free_proc_tree(proc_tree);
    }
    return NULL;
}

void free_proc_tree(proc_tree_t * proc_tree)
{
    free_proc_list(proc_tree);
}

int write_proc_tree(proc_tree_t * proc_tree, FILE * file)
{
    proc_t * init_proc;

    if ((init_proc = get_proc(proc_tree, INIT_PROC_PID)) == NULL) {
        return -1;
    }

    write_proc(init_proc, file, 0);

    return 0;
}

int write_proc(proc_t * proc, FILE * file, int level)
{
    const char * name = proc->name;
    fprintf(file, "%*s\n", 2*level + (int)strlen(name), name);

    if (proc->first_child != NULL) {
        // children indented another level
        level++;

        for (proc_child_t * node = proc->first_child; node; node = node->next) {
            proc_t * child = node->proc;
            int ret;

            if ((ret = write_proc(child, file, level)) != 0) {
                return ret;
            }
        }
    }

    return 0;
}

