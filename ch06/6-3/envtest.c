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
	printenv();

	return EXIT_SUCCESS;
}
