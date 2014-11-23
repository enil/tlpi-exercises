/**
 * @file	proc.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "proc.h"

// for read_uid
#include "misc.h"
// for snprintf
#include <stdio.h>
// for strncpy, strcmp, memset
#include <string.h>
// for true, false
#include <stdbool.h>
// for proc_error, PROC_ERROR_INVALID_FORMAT
#include "error.h"

char * get_status_file_path(proc_t * proc)
{
	static char buf[STRING_BUF_SIZE];
	int ret;

	// create filename
	ret = snprintf(buf, STRING_BUF_SIZE, "/proc/%lld/status", (long long)proc->pid);
	if (ret < 0 || ret >= STRING_BUF_SIZE) {
		// writing failed or string too long
		return NULL;
	}

	return buf;
}

proc_t * initialize_proc(proc_t * proc)
{
	memset(proc, 0, sizeof(proc_t));
	proc->cmd_name_set = false;
	proc->uid_set = false;
	proc->next = NULL;
	return proc;
}

void extract_proc_status(proc_t * proc, const char * label, const char * value)
{
	if (strcmp(label, "Name") == 0) {
		// status line describes the command name
		set_proc_cmd_name(proc, value);
	} else if (strcmp(label, "Uid") == 0) {
		// status line describes the UID
		uid_t uid;

		if (read_uid(value, &uid) == 0) {
			set_proc_uid(proc, uid);
		} else {
			proc_error = PROC_ERROR_INVALID_FORMAT;
		}
	}
}
