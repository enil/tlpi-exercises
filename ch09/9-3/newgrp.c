#include "newgrp.h"

// for size_t, gid_t
#include <sys/types.h>
// for setgroups, setgrent, getgrent, endgrent
#include <grp.h>
// for NGROUPS_MAX
#include <limits.h>
// for strcmp
#include <string.h>
// for bool, true, false
#include <stdbool.h>
// for errno, ENOMEM
#include <errno.h>
// for assert
#include <assert.h>

/**
 * The length of the groups buffer.
 */
#define NG_GROUPS_SIZE (NGROUPS_MAX + 1)

/**
 * Checks if a user is member of a group.
 */
static bool ng_check_group_member(const char * username, struct group * ent);

int ng_initgroups(const char * user, gid_t group)
{
	// initialize group list with the specified group
	gid_t groups[NG_GROUPS_SIZE] = { group };
	size_t num_groups = 1;
	struct group * ent;
	int ret = 0;

	// rewind to the first group
	setgrent();

	// find the user in the groups file
	while ((ent = getgrent()) != NULL) {
		if (ng_check_group_member(user, ent)) {
			if (num_groups == NG_GROUPS_SIZE) {
				// too many groups
				ret = -1;
				errno = -ENOMEM;
				goto out;
			}
			// add the current group
			groups[num_groups++] = ent->gr_gid;
		}
	}

	// try to set the groups
	ret = setgroups(num_groups, groups);

out:
	endgrent();
	return ret;
}

bool ng_check_group_member(const char * user, struct group * ent)
{
	// find the user in the list of users
	for (char ** current_user_p = ent->gr_mem; *current_user_p; ++current_user_p) {
		if (strcmp(user, *current_user_p) == 0) {
			// usernames match
			return true;
		}
	}

	return false;
}

