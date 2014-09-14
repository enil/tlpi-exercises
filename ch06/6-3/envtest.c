 /**
 * @file	testenv.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "env.h"

// for EXIT_SUCCESS
#include <stdlib.h>
// for printf
#include <stdio.h>
// for true, false
#include <stdbool.h>
// for assert
#include <assert.h>

extern char ** environ;

/**
 * Prints the environment variables.
 */
static void printenv(void)
{
	for (char ** env = environ; *env; ++env) {
		printf("%s\n", *env);
	}
}

int main(int argc, char * argv[])
{
	// always set path
	assert(newsetenv("PATH", "/opt/local/bin", true) == 0);

	// try to set user (will not succeed)
	assert(newsetenv("USER", "jsmith", false) == 0);

	// set new variables
	assert(newsetenv("foo", "bar", true) == 0);
	assert(newsetenv("hello", "world", true) == 0);

	assert(newunsetenv("hello") == 0);

	printenv();

	return EXIT_SUCCESS;
}
