 /**
 * @file	atomapp.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for exit, strtol, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, perror, stderr
#include <stdio.h>
// for strcmp
#include <string.h>
// for open, O_WRONLY, O_APPEND
#include <fcntl.h>
// for close, lseek, SEEK_END
#include <unistd.h>
// for S_IRUSR, S_IWUSR
#include <sys/stat.h>
// for errno
#include <errno.h>
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
	if (argc == 4) {
		if (strcmp(argv[3], "x") != 0) {
			return false;
		} else {
			options->append = false;
		}
	} else {
		options->append = true;
	}

	// success
	return true;
}

/**
 * Writes bytes to the file.
 */
static int write_bytes(int fd, struct opts options)
{
	for (int pos = 0; pos < options.num_bytes; ++pos) {
		if (!options.append) {
			// position at the end of the file
			if (lseek(fd, 0, SEEK_END) == -1) {
				return errno;
			}
		}
		if (write(fd, "x", 1) != 1) {
			return errno;
		}
	}

	fsync(fd);

	// success
	return 0;
}

int main(int argc, char * argv[])
{
	struct opts options;
	int fd = -1;

	if (!read_opts(&options, argc, argv)) {
		usage();
		exit(EXIT_FAILURE);
	}

	if ((fd = open(options.filename, O_CREAT | O_WRONLY | (options.append ? O_APPEND : 0), S_IRUSR | S_IWUSR)) == -1) {
		perror("Opening file");
		goto fail;
	}

	if (write_bytes(fd, options) != 0) {
		perror("Writing file");
		goto fail;
	}

	if (close(fd) == -1) {
		perror("Closing file");
		// don't attempt to close file again
		fd = -1;
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	if (fd != -1) {
		close(fd);
	}
	return EXIT_FAILURE;
}
