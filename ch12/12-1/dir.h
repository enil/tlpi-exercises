/**
 * @file	dir.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef DIR_H
#define DIR_H

// for bool
#include <stdbool.h>
// for struct dirent
#include <dirent.h>

/**
 * Callback for filtering entries in a directory.
 */
typedef bool (* filter_dir_entry)(struct dirent * entry, const char * path, void * data);

typedef bool (* process_dir_entry)(struct dirent * entry, const char * path, void * data);

/**
 * Walks and process the items of a directory.
 */
extern int walk_dir(const char * dir_path, filter_dir_entry filter, process_dir_entry process, void * data);

#endif

