/**
 * @file	ptree.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef PTREE_PTREE_H
#define PTREE_PTREE_H

// for proc_list_t
#include "plist.h"
// for FILE
#include <stdio.h>

typedef proc_list_t proc_tree_t;

/**
 * Creates a process tree structure.
 */
extern proc_tree_t * create_proc_tree(void); 

/**
 * Frees a process tree structure.
 */
extern void free_proc_tree(proc_tree_t * proc_tree);

/**
 * Writes a process tree structure.
 */
extern int write_proc_tree(proc_tree_t * proc_tree, FILE * file);
 
#endif

