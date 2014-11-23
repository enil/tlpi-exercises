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
		case PROC_ERROR_INVALID_ARGS:
			return "Invalid arguments";
		case PROC_ERROR_NO_SUCH_USER:
			return "User not found";
	}

	return "Unknown error";
}

void print_proc_error(const char * message)
{
	if (message) {
		fprintf(stderr, "%s: %s\n", message, get_proc_error_str(proc_error));
	} else {
		fprintf(stderr, "%s\n", get_proc_error_str(proc_error));
	}
}

