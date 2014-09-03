 /**
 * @file	cp.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for malloc, free, NULL, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, stderr, size_t
#include <stdio.h>
// for strnlen
#include <string.h>
// for open, close, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <fcntl.h>
// for S_IRUSR, S_IWUSER, S_IRGRP, S_IWGRP
#include <sys/stat.h>
// for close, read, write, lseek, SEEK_CUR
#include <unistd.h>
// for errno
#include <errno.h>

/** The mode for new files. */
#define DEST_FILE_MODE (mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

/** Size of the read buffer. */
#define BUFSIZE 1024

/**
 * Prints usage.
 */
static void usage(void)
{
	fprintf(stderr, "Usage: cp source_file target_file\n");
}

/**
 * Prints the latest error and quit.
 */
static void __attribute__((__noreturn__)) error(void)
{
	if (errno) {
		perror("tee");
	} else {
		fprintf(stderr, "Unknown error\n");
	}
	exit(EXIT_FAILURE);
}

/**
 * Skips zeros in a buffer and return the number of skipped characters.
 */
static int skip_zeros(char ** buf_p, size_t * buf_len_p)
{
	int num_skipped = 0;

	// loop until first non-zero character or end of buffer
	while (num_skipped < *buf_len_p && **buf_p == '\0') {
		(*buf_p)++;
		num_skipped++;
	}

	*buf_len_p -= num_skipped;
	return num_skipped;
}

/**
 * Copies data from the source file to the destination file.
 */
static int copy(int src_fd, int dest_fd)
{
	char * buf = NULL;
	size_t num_read;

	if ((buf = (char *)malloc(sizeof(char) * BUFSIZE)) == NULL) {
		goto fail;
	}

	while ((num_read = read(src_fd, buf, BUFSIZE)) > 0) {
		char * left = buf;
		size_t num_left = num_read;

		while (num_left > 0) {
			int len;

			// see if the buffert starts with any zeros
			size_t num_skipped = skip_zeros(&left, &num_left);
			if (num_skipped) {
				// skip zeros
				lseek(dest_fd, num_skipped, SEEK_CUR);
			}

			// count the number of bytes until the next zero
			len = strnlen(left, num_left);
			num_left -= len;

			// write the bytes until the next zero
			if (write(dest_fd, left, len) != len) {
				goto fail;
			}
		}
	}

	// check if finished because read failed
	if (num_read == -1) {
		goto fail;
	}

	free(buf);

	// successful
	return 0;

fail:
	if (buf) {
		free(buf);
	}
	return errno;
}

int main(int argc, char * argv[])
{
	char * src_filename, * dest_filename;
	int src_fd = -1, dest_fd = -1;

	if (argc != 3) {
		usage();
		exit(EXIT_FAILURE);
	}

	src_filename = argv[1];
	dest_filename = argv[2];

	src_fd = open(src_filename, O_RDONLY);
	if (src_fd == -1) {
		goto fail;
	}
	dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, DEST_FILE_MODE);
	if (dest_fd == -1) {
		goto fail;
	}

	// copy the data
	if (copy(src_fd, dest_fd) != 0) {
		goto fail;
	}

	if (close(dest_fd) != 0) {
		// couldn't close file, simply mark it as closed
		dest_fd = -1;
		goto fail;
	}
	if (close(src_fd) != 0) {
		// couldn't close file, simply mark it as closed
		src_fd = -1;
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	if (src_fd != -1) {
		close(src_fd);
	}
	error();
}

