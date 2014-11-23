#ifndef PROC_H
#define PROC_H

// for is_numeric_string
#include "misc.h"
// for pid_t, uid_t
#include <sys/types.h>
// for strncopy
#include <string.h>
// for struct dirent, DT_DIR
#include <dirent.h>
// for bool, true
#include <stdbool.h>

/**
 * The length of normal string buffers.
 */
#define STRING_BUF_SIZE 256

/**
 * Description of a process.
 */
typedef struct proc_t {
	/** The PID. */
	pid_t pid;

	/** Whether cmd_name has been set. */
	bool cmd_name_set;

	/** The short command name. */
	char cmd_name[STRING_BUF_SIZE];

	/** Whether uid has been set. */
	bool uid_set;

	/** The UID of the user owning the process. */
	uid_t uid;

	/** The next process description in the list. */
	struct proc_t * next;
} proc_t;

/**
 *  Checks if a directory entry is a directory for a process.
 */
static inline bool is_proc_entry(struct dirent * entry)
{
	// only accept directories with numeric names
	return is_numeric_string(entry->d_name) && entry->d_type == DT_DIR;
}

/**
 * Sets the PID of a process.
 */
static inline void set_proc_pid(proc_t * proc, pid_t pid)
{
	proc->pid = pid;
}

/**
 * Sets the command name of a process.
 */
static inline void set_proc_cmd_name(proc_t * proc, const char * cmd_name)
{
	strncpy(proc->cmd_name, cmd_name, STRING_BUF_SIZE);
	proc->cmd_name_set = true;
}

/**
 * Sets the command name of a process.
 */
static inline void set_proc_uid(proc_t * proc, uid_t uid)
{
	proc->uid = uid;
	proc->uid_set = true;
}

/**
 * Checks whether a process description is complete.
 */
static inline bool proc_completed(proc_t * proc)
{
	return proc->cmd_name_set && proc->uid_set;
}

/**
 * Returns the path to the status file of a process.
 */
extern char * get_status_file_path(proc_t * proc);

/**
 * Initializes a process description.
 */
extern proc_t * initialize_proc(proc_t * proc);

/**
 * Extracts data from a label and value and sets it in a process description.
 */
extern void extract_proc_status(proc_t * proc, const char * label, const char * value);

#endif

