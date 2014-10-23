#include "newpwd.h"

// for NULL
#include <stdlib.h>
// for getpwent, setpwent, endpwent
#include <pwd.h>
// for memcpy, strncpy, strcmp
#include <string.h>

/**
 * The size of string bufferts for copied password objects.
 */
#define NL_STR_BUFFER_SIZE 100

/**
 * Copies the data for a struct password object.
 */
static struct passwd * np_copy_passwd(struct passwd * old_pwd);

struct passwd * np_getpwnam(const char * name)
{
	struct passwd * pwd;
	
	// rewind password entries if needed
	setpwent();

	// find the user
	while ((pwd = getpwent()) != NULL) {
		if (strcmp(pwd->pw_name, name) == 0) {
			// username matches
			break;
		}
	}

	if (pwd != NULL) {
		// copy the password object
		pwd = np_copy_passwd(pwd);
	}
	// reset password entry
	endpwent();

	return pwd;
}

static struct passwd * np_copy_passwd(struct passwd * old_pwd)
{
	// copied strings
	static char new_name[NL_STR_BUFFER_SIZE];
	static char new_passwd[NL_STR_BUFFER_SIZE];
	static char new_gecos[NL_STR_BUFFER_SIZE];
	static char new_dir[NL_STR_BUFFER_SIZE];
	static char new_shell[NL_STR_BUFFER_SIZE];
	// the copied password object
	static struct passwd new_pwd;

	// copy the password object and its strings
	memcpy(&new_pwd, old_pwd, sizeof(struct passwd));
	new_pwd.pw_name = strncpy(new_name, old_pwd->pw_name, NL_STR_BUFFER_SIZE);
	new_pwd.pw_passwd = strncpy(new_passwd, old_pwd->pw_passwd, NL_STR_BUFFER_SIZE);
	new_pwd.pw_gecos = strncpy(new_gecos, old_pwd->pw_gecos, NL_STR_BUFFER_SIZE);
	new_pwd.pw_dir = strncpy(new_dir, old_pwd->pw_dir, NL_STR_BUFFER_SIZE);
	new_pwd.pw_shell = strncpy(new_shell, old_pwd->pw_shell, NL_STR_BUFFER_SIZE);

	// return statically allocated copy
	return &new_pwd;
}

