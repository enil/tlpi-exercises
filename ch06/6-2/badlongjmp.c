 /**
 * @file	badlongjmp.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for EXIT_SUCCESS
#include <stdlib.h>
// for printf
#include <stdio.h>
// for setjmp, longjmp, jmp_buf
#include <setjmp.h>

/** The environment for the long jump into b. */
static jmp_buf b_env;

static void b(void)
{
	printf("entering b\n");

	if (setjmp(b_env) == 0) {
		printf("b: jump set in b\n");
	} else {
		printf("b: jumped into b\n");
	}

	printf("leaving b\n");
}

static void a(void)
{
	printf("entering a\n");

	// call the function where the jump is specified
	b();

	// jump into b
	printf("a: jumping to b\n");
	longjmp(b_env, 1);
	printf("a: jumped to b\n");

	printf("leaving a\n");
}


int main(int argc, char * argv[])
{
	a();
	return EXIT_SUCCESS;
}

