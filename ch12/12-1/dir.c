#include "dir.h"

// for opendir, closedir, struct dirent
#include <dirent.h>
// for snprintf
#include <stdio.h>
// for errno
#include <errno.h>

/** The size of the path entry buffer. */
#define PATH_BUF_SIZE 512

/**
 * Creates a full path from a directory and a directory entry.
 */
static char * dir_entry_path(const char * dir_path, struct dirent * entry);

int walk_dir(const char * dir_path, filter_dir_entry filter, process_dir_entry process, void * data)
{
	DIR * dir = NULL;
	struct dirent * entry;
	int ret = 0;

	if ((dir = opendir(dir_path)) == NULL) {
		// failed to open dir
		ret =  -1;
		goto out;
	}

	while ((entry = readdir(dir)) != NULL) {
		const char * path = dir_entry_path(dir_path, entry);

		if (path == NULL) {
			// failed to write path
			ret = -1;
			goto out;
		}

		// process the entry if it passes the filter
		if (filter(entry, path, data)) {
			if (!process(entry, path, data)) {
				break;
			}
		}
	}

	if (errno) {
		// error reading a directory
		ret = -1;
		goto out;
	}

out:
	if (dir) {
		if (closedir(dir) != 0) {
			ret = -1;
		}
	}
	return ret;
}

char * dir_entry_path(const char * dir_path, struct dirent * entry)
{
	static char buf[PATH_BUF_SIZE];
	int ret;

	// create path
	ret = snprintf(buf, PATH_BUF_SIZE, "%s/%s", dir_path, entry->d_name);
	if (ret < 0 || ret >= PATH_BUF_SIZE) {
		// writing failed or string too long
		return NULL;
	}

	return buf;
}

