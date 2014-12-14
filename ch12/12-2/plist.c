/**
 * @file	plist.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "plist.h"

// for read_proc, free_proc, proc_t
#include "proc.h"
// for malloc, free
#include <stdlib.h>
// for snprintf, fopen, fclose, FILE
#include <stdio.h>
// for memset
#include <string.h>
// for opendir, closedir, struct dirent
#include <dirent.h>
// for isdigit
#include <ctype.h>
// for bool
#include <stdbool.h>
// for errno
#include <errno.h>
// for assert
#include <assert.h>

/** The size of the status file path buffer. */
#define PTREE_STATUS_FILE_PATH_SIZE 272

/**
 * Opens and parses a proc file.
 */
static proc_t * parse_proc_file(const char * path);

/**
 * Checks if a string only consists of decimal digits.
 */
static bool is_numeric_string(const char * str);

/**
 * Gets the absolute path for a process status file.
 */
static const char * get_status_file_path(const char * filename);

proc_list_t * create_proc_list(void)
{
	proc_list_t * proc_list = NULL;
	DIR * dir = NULL;
	struct dirent * entry = NULL;

	if ((proc_list = alloc_proc_list()) == NULL) {
		goto fail;
	}

    if ((dir = opendir("/proc")) == NULL) {
		goto fail;
    }

    while ((entry = readdir(dir)) != NULL) {
        const char * filename = entry->d_name;

        if (is_numeric_string(filename)) {
            const char * path;
			proc_t * proc;

            if ((path = get_status_file_path(filename)) == NULL) {
				goto fail;
            }
			if ((proc = parse_proc_file(path)) == NULL) {
				goto fail;
			}

			put_proc(proc_list, proc);
        }
    }

    if (errno != 0) {
		goto fail;
    }

    if (closedir(dir) == -1) {
		goto fail;
    }

	return proc_list;

fail:
    if (dir != NULL) {
		closedir(dir);
    }
	if (proc_list != NULL) {
		free_proc_list(proc_list);
	}
	return NULL;
}

proc_list_t * alloc_proc_list(void)
{
	proc_list_t * proc_list = NULL;

	if ((proc_list = (proc_list_t *)malloc(sizeof(proc_list_t))) == NULL) {
		return NULL;
	}
	memset(proc_list, 0, sizeof(proc_list_t));

	// store the size
	proc_list->size = PTREE_PROC_MAX;

	return proc_list;
}

void free_proc_list(proc_list_t * proc_list)
{
    proc_t * proc;
	EACH_PROC_LIST(proc_list, proc) {
		free_proc(proc);
	}
	free(proc_list);
}

proc_t * get_proc(proc_list_t * proc_list, pid_t pid)
{
	assert(pid < PTREE_PROC_MAX);
	return proc_list->procs[pid];
}

void put_proc(proc_list_t * proc_list, proc_t * proc)
{
	assert(proc->pid < PTREE_PROC_MAX);
	proc_list->procs[proc->pid] = proc;
}

proc_t * parse_proc_file(const char * path)
{
	FILE * file = NULL;
	proc_t * proc = NULL;

	if ((file = fopen(path, "r")) == NULL) {
		goto fail;
	}

	if ((proc = read_proc(file)) == NULL) {
		goto fail;
	}

	if (fclose(file) != 0) {
		goto fail;
	}

	return proc;

fail:
	if (proc != NULL) {
		free_proc(proc);
	}
	if (file != NULL) { 
		fclose(file);
	}
	return NULL;
}

bool is_numeric_string(const char * str)
{
    const char * cur = str;

    while (*cur != '\0' && isdigit(*cur)) {
        cur++;
    }
    return *cur == '\0';
}

const char * get_status_file_path(const char * filename)
{
    static char path[PTREE_STATUS_FILE_PATH_SIZE];
    int len;

    len = snprintf(path, PTREE_STATUS_FILE_PATH_SIZE, "/proc/%s/status", filename);
    if (len < 0 || len >= PTREE_STATUS_FILE_PATH_SIZE) {
        return NULL;
    }

    return path;
}

