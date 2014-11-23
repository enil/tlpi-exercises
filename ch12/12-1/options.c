/**
 * @file	options.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "options.h"

// for getpwnam
#include <pwd.h>
// for uid_t, struct passwd
#include <sys/types.h>
// for memset
#include <string.h>
// for proc_error
#include "error.h"

/**
 * Finds the UID of a user.
 */
static int get_user_uid(const char * username, uid_t * uid);

int read_options(proc_options_t * options, int argc, const char * argv[])
{
	const char * username;

	// clear options
	memset(options, 0, sizeof(proc_options_t));

	if (argc != 2) {
		// wrong number of arguments
		proc_error = PROC_ERROR_INVALID_ARGS;
		return -1;
	}

	// get the user name argument:w
	username = argv[1];
	if (get_user_uid(username, &options->uid) != 0) {
		// can't find a PID for the user
		proc_error = PROC_ERROR_NO_SUCH_USER;
		return -1;
	}

	return 0;
}

int get_user_uid(const char * username, uid_t * uid)
{
	struct passwd * password;

	if ((password = getpwnam(username)) == NULL) {
		// user not found
		return -1;
	}

	// return user UID
	*uid = password->pw_uid;
	return 0;
}

