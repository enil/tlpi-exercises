#include "misc.h"

// for isdigit, isspace
#include <ctype.h>
// for strtol
#include <stdlib.h>
// for LONG_MIN, LONG_MAX
#include <limits.h>

/**
 * Reads a long number from a string.
 */
static int read_long(const char * str, long * val_p);

bool is_numeric_string(const char * str)
{
	while (isdigit(*(str)) && str++);
	return *str == '\0';
}

int read_uid(const char * str, uid_t * uid_p)
{
	long val;

	if (read_long(str, &val) != 0) {
		return -1;
	}

	*uid_p = (uid_t)val;
	return 0;
}

int read_pid(const char * str, pid_t * pid)
{
	long val;

	if (read_long(str, &val) != 0) {
		return -1;
	}

	*pid = (pid_t)val;
	return 0;
}

int read_long(const char * str, long * val_p)
{
	char * end;
	long val = strtol(str, &end, 10);

	if ((*end != '\0' && !isspace(*end)) || val == LONG_MIN || val == LONG_MAX) {
		// not a valud UID string
		return -1;
	}

	*val_p = val;
	return 0;
}

