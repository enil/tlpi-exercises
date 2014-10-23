// for np_getpwnam
#include "newpwd.h"
// for exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for printf, fprintf, stderr
#include <stdio.h>
// for struct passwd
#include <pwd.h>
// for strcmp
#include <string.h>

/**
 * Prints information about a user.
 */
static void print_passwd(struct passwd * pwd)
{
	printf("%10s : %s\n", "Username", pwd->pw_name);
	if (pwd->pw_passwd && strcmp("x", pwd->pw_passwd)) {
		printf("%10s : %s\n", "Password", pwd->pw_passwd);
	}
	printf("%10s : %ld\n", "UID", (long)pwd->pw_uid);
	printf("%10s : %ld\n", "GID", (long)pwd->pw_gid);
	printf("%10s : %s\n", "GECOS", pwd->pw_gecos);
	printf("%10s : %s\n", "Home", pwd->pw_dir);
	printf("%10s : %s\n", "Shell", pwd->pw_shell);
}

int main(int argc, char * argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: pwdtest usernames\n");
		exit(EXIT_FAILURE);
	}

	char ** usernames = &argv[1];

	// print information about all usernames
	for (char ** username_p = usernames; *username_p != NULL; ++username_p) {
		struct passwd * pwd = np_getpwnam(*username_p);
		if (pwd != NULL) {
			print_passwd(pwd);
		} else {
			fprintf(stderr, "User %s not found\n", *username_p);
		}
	}

	exit(EXIT_SUCCESS);
}

