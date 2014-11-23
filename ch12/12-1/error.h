#ifndef ERROR_H
#define ERROR_H

/** An error code for the process list program. */
extern int proc_error;

/** No error. */
#define PROC_ERROR_SUCCESS 0

/** Invalid format in status file. */
#define PROC_ERROR_INVALID_FORMAT 1

/** Invalid command-line arguments. */
#define PROC_ERROR_INVALID_ARGS 2

/** No user with the provided name. */
#define PROC_ERROR_NO_SUCH_USER 3

/**
 * Gets an error message for a process list error.
 */
extern const char * get_proc_error_str(int error);

/**
 * Prints the process list error.
 */
extern void print_proc_error(const char * message);

#endif

