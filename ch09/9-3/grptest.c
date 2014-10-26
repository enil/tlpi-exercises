#include "newgrp.h"
// for printf, fprintf, perror, stderr
#include <stdio.h>
// for exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for getpwnam, struct passwd
#include <pwd.h>
// for getgrgid, struct group
#include <grp.h>
// for gid_t
#include <sys/types.h>
// for getgroups
#include <unistd.h>
// for strncpy
#include <string.h>
// for NGROUPS_MAX
#include <limits.h>

/**
 * The size of the group name buffer.
 */
#define GROUP_NAME_SIZE 128

/**
 * Gets the name of a group.
 */
static const char * get_group_name(gid_t gid)
{
	static char name[GROUP_NAME_SIZE];
	struct group * grp;

	if ((grp = getgrgid(gid)) == NULL) {
		return NULL;
	}

	// copy the group name;
	strncpy(name, grp->gr_name, GROUP_NAME_SIZE);
	name[GROUP_NAME_SIZE-1] = '\0';

	return name;
}

/**
 * Prints a list of the groups the process is a member of.
 */
static int print_groups(void)
{
	gid_t groups[NGROUPS_MAX + 1];
	size_t num_groups;

	if ((num_groups = getgroups(NGROUPS_MAX + 1, groups)) == -1) {
		return -1;
	}

	// print all the groups
	for (int pos = 0; pos < num_groups; ++pos) {
		const char * name;
		gid_t gid = groups[pos];

		if ((name = get_group_name(gid)) == NULL) {
			name = "[unknown]";
		}

		// print the group info
		printf("%ld:\t%s\n", (long)gid, name);
	}
}

/**
 * Gets the primary group of a user.
 */
gid_t get_primary_group(const char * user)
{
	struct passwd * pwd;

	if ((pwd = getpwnam(user)) == NULL) {
		// user not found
		return (gid_t)-1;
	}

	return pwd->pw_gid;
}

int main(int argc, char * argv[])
{
	const char * user;
	gid_t group;

	if (argc != 2) {
		fprintf(stderr, "Usage: grptest user\n");
		exit(EXIT_FAILURE);
	}

	user = argv[1];
	if ((group = get_primary_group(user)) == -1) {
		fprintf(stderr, "Couldn't find user %s\n", user);
		exit(EXIT_FAILURE);
	}

	// set the groups for the user
	if (ng_initgroups(user, group) == -1) {
		perror("Setting groups");
		exit(EXIT_FAILURE);
	}

	// print the groups of the process to verify
	print_groups();

	return EXIT_SUCCESS;
}

