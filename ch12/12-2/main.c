/**
 * @file	main.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for fprintf, stdout, stderr
#include <stdio.h>
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for create_proc_tree, write_proc_tree, proc_tree_t
#include "ptree.h"

int main(int argc, const char * argv[])
{
	int ret = EXIT_SUCCESS;
    proc_tree_t * proc_tree = NULL;

    if ((proc_tree = create_proc_tree()) == NULL) {
        fprintf(stderr, "Creating process tree failed\n");
        ret = EXIT_FAILURE;
        goto out;
    }

    if (write_proc_tree(proc_tree, stdout) != 0) {
        fprintf(stderr, "Writing process tree failed\n");
        ret = EXIT_FAILURE;
        goto out;
    }

out:
    if (proc_tree != NULL) {
        free_proc_tree(proc_tree);
    }
	return ret;
}

