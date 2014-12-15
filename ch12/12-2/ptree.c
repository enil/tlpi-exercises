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
// malloc, free
#include <stdlib.h>
// for strcpy, strlen, strchr
#include <string.h>
// for bool
#include <stdbool.h>

/** The PID of the init process. */
#define INIT_PROC_PID 1

/** The number of spaces for every indent level in the tree. */
#define TREE_INDENT_SPACES 2

/** Size of a buffer to use with tree_repr_t. */
#define TREE_LINE_BUF_SIZE 120

// Visual representations of segments of a tree
#define TREE_REPR_FORK      " ├──"
#define TREE_REPR_CORNER    " └──"
#define TREE_RERP_VERTICAL  " │  "
#define TREE_REPR_BLANK     "    "
#define TREE_REPR_END       ""

/**
 * Context for visual representation of a process tree.
 */
typedef struct tree_repr_t {
    /** Start of the visualization of a line. */
    char * line_start;

    /** End of the visualization of a line. */
    char * line_end;
} tree_repr_t;

/**
 * Finds the next position of a tree representation line to write to.
 */
static char * get_repr_line_next(tree_repr_t * repr);

/**
 * Updates a part of the tree representation.
 */
static int update_repr_line(tree_repr_t * repr, char * next, const char * str);

/**
 * Writes the next line of the tree representation.
 */
static void write_repr_line(tree_repr_t * repr, FILE * file);

/**
 * Writes a process and its children.
 */
static int write_proc(proc_t * proc, FILE * file, tree_repr_t * repr);

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
    // buffer for storing the representation of the tree
    char line[TREE_LINE_BUF_SIZE] = TREE_REPR_BLANK;
    tree_repr_t repr = { .line_start = line, .line_end = line + TREE_LINE_BUF_SIZE };

    // the init process is the root node
    if ((init_proc = get_proc(proc_tree, INIT_PROC_PID)) == NULL) {
        return -1;
    }

    write_proc(init_proc, file, &repr);

    return 0;
}

char * get_repr_line_next(tree_repr_t * repr)
{
    return strchr(repr->line_start, '\0');
}

int update_repr_line(tree_repr_t * repr, char * next, const char * str)
{
    size_t len = strlen(str);
    // prevent overflow
    if (next + len >= repr->line_end) {
        return -1;
    }

    strcpy(next, str);
    return 0;
}

void write_repr_line(tree_repr_t * repr, FILE * file)
{
    fprintf(file, "%s", repr->line_start);
}


int write_proc(proc_t * proc, FILE * file, tree_repr_t * repr)
{
    const char * name = proc->name;
    // write the name
    fprintf(file, "%s\n", name);

    if (proc->first_child != NULL) {
        // find the end of the tree representation
        char * next = get_repr_line_next(repr);

        for (proc_child_t * node = proc->first_child; node; node = node->next) {
            proc_t * child = node->proc;
            bool is_last = node->next == NULL;
            bool has_children = child->first_child != NULL;

            // replace last segment of tree representation for the child
            if (update_repr_line(repr, next, is_last ? TREE_REPR_CORNER : TREE_REPR_FORK) != 0) {
                return -1;
            }

            // write the tree representation of child
            write_repr_line(repr, file);

            if (has_children) {
                // replace last segment of tree representation for the level below the child
                if (update_repr_line(repr, next, is_last ? TREE_REPR_BLANK : TREE_RERP_VERTICAL) != 0) {
                    return -1;
                }
            }

            // recursivly write tree for the child
            if (write_proc(child, file, repr) != 0) {
                return -1;
            }
        }

        // remove the last segment
        if (update_repr_line(repr, next, TREE_REPR_END) != 0) {
            return -1;
        }
    }

    return 0;
}

