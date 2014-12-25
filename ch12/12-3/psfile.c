/**
 * @file	psfile.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "psfile.h"

// for opendir, closedir, DT_DIR, DT_LNK, DIR, struct dirent
#include <dirent.h>
// for printf, asprintf
#include <stdio.h>
// for isdigit
#include <ctype.h>
// for realpath, free
#include <stdlib.h>
// for strcmp
#include <string.h>
// for bool
#include <stdbool.h>
// for errno
#include <errno.h>

/**
 * Matches a process for a certain open file.
 */
static int match_proc_files(const char * path, const char * dir_path, bool * matches);

/**
 * Checks if a string only consists of decimal digits.
 */
static bool is_numeric_string(const char * str);

/**
 * Gets the path for the fd path for a proc directory.
 */
static char * get_fd_dir_path(const char * proc_dir);

/**
 * Creates an absolute path from a path to a directory and a dirent.
 *
 * The dirent can be omitted.
 */
static char * create_full_path(const char * dir_path, struct dirent * entry);

int match_all_proc_files(const char * path)
{
    int ret = 0;
	DIR * dir = NULL;
	struct dirent * entry = NULL;
    char * absolute_path = NULL;

    // get the absolute path for the file
    if ((absolute_path = create_full_path(path, NULL)) == NULL) {
        ret = -1;
        goto out;
    }

    if ((dir = opendir("/proc")) == NULL) {
        ret = -1;
		goto out;
    }

    while ((entry = readdir(dir)) != NULL) {
        const char * dir_name = entry->d_name;

        if (entry->d_type == DT_DIR && is_numeric_string(dir_name)) {
            char * dir_path = NULL;
            bool matches = false;

            // get a path for the directory with the file descriptors
            if ((dir_path = get_fd_dir_path(dir_name)) == NULL) {
                ret = -1;
                goto out;
            }

            if ((ret = match_proc_files(path, dir_path, &matches)) != 0) {
                // might fail if process has died, just ignore
                errno = 0;
            }

            // print the PID of processes matching the file
            if (matches) {
                printf("%s\n", dir_name);
            }

            free(dir_path);
        }

    }

    if (errno != 0) {
        ret = -1;
		goto out;
    }


out:
    if (dir && closedir(dir) != 0) {
        ret = -1;
    }

    if (absolute_path) {
        free(absolute_path);
    }

    return ret;
}

int match_proc_files(const char * path, const char * dir_path, bool * matches)
{
    int ret = 0;
	DIR * dir = NULL;
	struct dirent * entry = NULL;

    *matches = false;

    if ((dir = opendir(dir_path)) == NULL) {
        ret = -1;
		goto out;
    }

    while ((entry = readdir(dir)) != NULL && !*matches) {
        const char * filename = entry->d_name;

        if (entry->d_type == DT_LNK && is_numeric_string(filename)) {
            char * absolute_path = NULL;

            // might fail if the file has been closed, just ignore
            if ((absolute_path = create_full_path(dir_path, entry)) != NULL) {
                if (strcmp(absolute_path, path) == 0) {
                    // file names matched
                    *matches = true;
                }

                free(absolute_path);
            }
        }
    }

    if (errno != 0) {
        ret = -1;
		goto out;
    }


out:
    if (dir && closedir(dir) != 0) {
        ret = -1;
    }

    return ret;
}

bool is_numeric_string(const char * str)
{
    const char * cur = str;

    while (*cur != '\0' && isdigit(*cur)) {
        cur++;
    }
    return *cur == '\0';
}

char * get_fd_dir_path(const char * proc_dir)
{
    char * buf;

    if (asprintf(&buf, "/proc/%s/fd", proc_dir) < 0) {
        return NULL;
    }

    return buf;
}

char * create_full_path(const char * dir_path, struct dirent * entry)
{
    char * path = NULL;
    char * absolute_path = NULL;

    if (entry) {
        // concatenate file name with directory path
        if (asprintf(&path, "%s/%s", dir_path, entry->d_name) < 0) {
            return NULL;
        }
    }

    absolute_path = realpath(path ? path : dir_path, NULL);
    errno = 0;

    if (path) {
        free(path);
    }

    return absolute_path;
}

