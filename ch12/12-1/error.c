#include "error.h"

int proc_error = PROC_ERROR_SUCCESS;

// for fprintf, stderr
#include <stdio.h>

const char * get_proc_error_str(int error)
{
	switch (error) {
		case PROC_ERROR_SUCCESS:
			return "No error";
		case PROC_ERROR_INVALID_FORMAT:
			return "Invalid format";
	}

	return "Unknown error";
}

void print_proc_error(const char * message)
{
	fprintf(stderr, "%s: %s\n", message, get_proc_error_str(proc_error));
}

