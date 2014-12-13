/**
 * @file	pchildren.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "pchildren.h"

// for proc_t
#include "proc.h"
// for get_proc, proc_list_t, EACH_PROC_LIST
#include "ptree.h"
// for malloc, free
#include <stdlib.h>
// for assert
#include <assert.h>

int populate_proc_children(proc_list_t * proc_list)
{
    proc_t * proc;

    EACH_PROC_LIST(proc_list, proc) {
        if (proc->ppid != 0) {
            proc_t * parent = get_proc(proc_list, proc->ppid);

            // parent might have died
            if (parent) {
                int ret;

                if ((ret = add_proc_child(parent, proc)) != 0) {
                    return ret;
                }
            }
        }
    }

    return 0;
}

int add_proc_child(proc_t * parent_proc, proc_t * child_proc)
{
    proc_child_t * child = NULL;
    proc_child_t * previous = parent_proc->last_child;

    if ((child = (proc_child_t *)malloc(sizeof(proc_child_t))) == NULL) {
        return -1;
    }

    child->proc = child_proc;
    child->next = NULL;

    if (previous != NULL) {
        // insert after the last child node
        parent_proc->last_child = previous->next = child;
    } else {
        // no child nodes
        parent_proc->last_child = parent_proc->first_child  = child;
    }

    return 0;
}

void free_child(proc_child_t * child)
{
    free(child);
}

