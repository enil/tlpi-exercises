/**
 * @file	main.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for fprintf, stderr
#include <stdio.h>
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for create_proc_list, free_proc_list, proc_list_t
#include "ptree.h"
// for populate_proc_children, EACH_PROC_CHILD
#include "pchildren.h"

int main(int argc, const char * argv[])
{
	int ret = EXIT_SUCCESS;
    proc_list_t * proc_list = NULL;
    proc_t * init_proc = NULL;
    proc_t * child = NULL;

    if ((proc_list = create_proc_list()) == NULL) {
        fprintf(stderr, "Creating process list failed\n");
        ret = EXIT_FAILURE;
        goto out;
    }

    if (populate_proc_children(proc_list) != 0) {
        fprintf(stderr, "Populating process children failed\n");
        ret = EXIT_FAILURE;
        goto out;
    }

    if ((init_proc = get_proc(proc_list, 1)) == NULL) {
        fprintf(stderr, "Could not fetch init process\n");
        ret = EXIT_FAILURE;
        goto out;
    }

    EACH_PROC_CHILD(init_proc, child) {
        printf("pid: %6lld ppid: %-6lld name: %s\n", (long long)child->pid, (long long)child->ppid, child->name);
    }

out:
    if (proc_list != NULL) {
        free_proc_list(proc_list);
    }
	return ret;
}

