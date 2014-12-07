/**
 * @file	main.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for printf, fprintf, fopen, fclose, stderr
#include <stdio.h>
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for read_proc, proc_t
#include "proc.c"

int main(int argc, const char * argv[])
{
	int ret = EXIT_SUCCESS;
	FILE * f;
	proc_t * proc;

	f = fopen("/proc/1/status", "r");
	if (f == NULL) {
		perror("opening file");
		ret = EXIT_FAILURE;
		goto out;
	}
	if ((proc = read_proc(f)) == NULL) {
		fprintf(stderr, "parsing process file failed\n");
		ret = EXIT_FAILURE;
		goto out;
	}
	if (fclose(f) != 0) {
		fprintf(stderr, "closing file\n");
		ret = EXIT_FAILURE;
		goto out;
	}

	printf("ppid: %lld, name: %s\n", (long long)proc->ppid, proc->name);

out:
	return ret;
}

