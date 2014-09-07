// for exit, strtol, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, stderr
#include <stdio.h>
// for bool, true, false
#include <stdbool.h>

/**
 * Options from the command line.
 */
struct opts {
	/** The filename. */
	char * filename;

	/** The number of bytes to write. */
	int num_bytes;

	/** Whether O_APPEND should be used. */
	bool append;
};

/**
 * Prints usage.
 */
static void usage(void)
{
	fprintf(stderr, "Usage: atomic_append filename num-bytes [x]\n");
}

/**
 * Reads command line options.
 */
static bool read_opts(struct opts * options, int argc, char * argv[])
{
	char * end;

	if (argc < 3 || argc > 4) {
		// to few or many options supplied
		return false;
	}

	options->filename = argv[1];

	options->num_bytes = strtol(argv[2], &end, 10);
	if (*end != '\0') {
		// 2nd argument not a number
		return false;
	}

	// use 3rd argument to ommit O_APPEND
	options->append = argc != 4;

	// success
	return true;
}

int main(int argc, char * argv[])
{
	struct opts options;

	if (!read_opts(&options, argc, argv)) {
		usage();
		exit(EXIT_FAILURE);
	}

	printf("filename: %s\n", options.filename);
	printf("num-bytes: %i\n", options.num_bytes);
	printf("append: %s\n", options.append ? "yes" : "no");

	return 0;
}
