 /**
 * @file	env.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "env.h"

// for asprintf
#include <stdio.h>
// for getenv
#include <stdlib.h>
// for strlen, strstr
#include <string.h>
// for bool
#include <stdbool.h>
// for errno, ENOMEM
#include <errno.h>

extern char ** environ;

/**
 * Checks whether the name of an environment variable string matches.
 */
static bool match_env_name(const char * value, const char * name)
{
	char * match = strstr(value, name);

	// starts with name and is followed by '='
	return (match == value && match[strlen(name)] == '=');
}

int newsetenv(const char * name, const char * value, int overwrite)
{
	char * buf;
	int ret;

	if (!overwrite && getenv(name)) {
		// already set, don't overwrite
		return 0;
	}

	if (asprintf(&buf, "%s=%s", name, value) == -1) {
		// could not allocate buffer
		errno = ENOMEM;
		return -1;
	}

	ret = putenv(buf);

	return ret;
}

int newunsetenv(const char * name)
{
	if (environ == NULL || *environ == NULL) {
		// environment already empty
		return 0;
	}

	// the destination for the next variable to put
	char ** dest = environ;

	// recreate environ by removing matching variables
	for (char ** cur = environ; *cur; ++cur) {
		if (!match_env_name(*cur, name)) {
			// put back the variable at the destination
			*dest = *cur;

			dest++;
		}
	}

	// indicate new end of the environment
	*dest = NULL;

	// success
	return 0;
}

