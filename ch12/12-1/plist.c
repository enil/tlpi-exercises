// for is_proc_entry, proc_completed, extract_proc_status
#include "proc.h"
// for walk_dir
#include "dir.h"
// for parse_status_file
#include "status.h"
// for read_options, proc_options_t
#include "options.h"
// for read_pid
#include "misc.h"
// for proc_error, PROC_ERROR_SUCCESS
#include "error.h"
// for printf, perror
#include <stdio.h>
// for EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for errno
#include <errno.h>

/**
 * Filters file names to determin if the entry is a directory for a process.
 */
static bool filter_proc(struct dirent * entry, const char * path, void * data)
{
	return is_proc_entry(entry);
}

/**
 * Processes a line in a process status file.
 */
static bool process_line(const char * label, const char * value, void * data)
{
	proc_t * proc = data;

	// try to extract wanted data from the status label and value
	extract_proc_status(proc, label, value);
	// don't continue when all wanted data is set
	return !proc_completed(proc);
}

/**
 * Processes a status file and prints information about the process.
 */
static bool process_status(struct dirent * entry, const char * path, void * data)
{
	proc_options_t * options = data;
	const char * status_path;
	proc_t proc;

	// initialize the new process
	initialize_proc(&proc);

	if (read_pid(entry->d_name, &proc.pid) != 0) {
		// could not parse PID, skip process file
		return true;
	}

	// get the path for the status file
	if ((status_path = get_status_file_path(&proc)) == NULL) {
		// could not allocate memory, stop
		return false;
	}

	// try to parse the lines of the status file
	parse_status_file(status_path, process_line, &proc);
	if (proc_error) {
		// error while parsing status file, skip
		print_proc_error("Parsing status file");
		// reset error
		proc_error = PROC_ERROR_SUCCESS;
		return true;
	} else if (proc_completed(&proc) && proc.uid == options->uid) {
		// print process information
		printf("%-6lld %s\n", (long long)proc.pid, proc.cmd_name);
	}

	return true;
}

int main(int argc, const char * argv[])
{
	proc_options_t options;

	if (read_options(&options, argc, argv) != 0) {
		if (proc_error != PROC_ERROR_SUCCESS) { 
			print_proc_error(NULL);
		} else {
			fprintf(stderr, "Usage: plist username\n");
		}
		return EXIT_FAILURE;
	}

	// process status files
	walk_dir("/proc", filter_proc, process_status, &options);

	// print any errors if parsing failed
	if (errno) {
		perror("Parsing status file");
		return EXIT_FAILURE;;
	} else if (proc_error) {
		return EXIT_FAILURE;;
	}

	return EXIT_SUCCESS;
}

