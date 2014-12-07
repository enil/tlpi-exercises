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
// for create_proc_list, free_proc_list, EACH_PROC_LIST, proc_list_t
#include "ptree.h"

int main(int argc, const char * argv[])
{
	int ret = EXIT_SUCCESS;
    proc_list_t * proc_list = NULL;

    if ((proc_list = create_proc_list()) == NULL) {
        fprintf(stderr, "Creating process list failed\n");
        ret = EXIT_FAILURE;
        goto out;
    }

    // print information about each process
    EACH_PROC_LIST(proc_list, proc) {
        printf("pid: %-6lld ppid: %-6lld name: %s\n", (long long)proc->pid, (long long)proc->ppid, proc->name);
    }

out:
    if (proc_list != NULL) {
        free_proc_list(proc_list);
    }
	return ret;
}

