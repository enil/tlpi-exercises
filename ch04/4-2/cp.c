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
static size_t skip_zeros(char ** buf_p, size_t * buf_len_p)
{
	size_t num_skipped = 0;

	// loop until first non-zero character or end of buffer
	while (num_skipped < *buf_len_p && **buf_p == '\0') {
		(*buf_p)++;
		num_skipped++;
	}

	*buf_len_p -= num_skipped;
	return num_skipped;
}

/**
 * Writes nonzeros from a buffer and return the number of written characters.
 */
static ssize_t write_nonzeros(int fd, char ** buf_p, size_t * buf_len_p)
{
	size_t len;

	// count the number of bytes until the next zero
	len = strnlen(*buf_p, *buf_len_p);

	if (len) {
		// write the bytes until the next zero
		if (write(fd, *buf_p, len) != len) {
			return -errno;
		}
		*buf_len_p -= len;
		*buf_p += len;
	}
	return len;
}

/**
 * Copies data from the source file to the destination file.
 */
static int copy(int src_fd, int dest_fd)
{
	char * buf = NULL;
	size_t num_read;
	size_t num_skip = 0;

	if ((buf = (char *)malloc(sizeof(char) * BUFSIZE)) == NULL) {
		goto fail;
	}

	while ((num_read = read(src_fd, buf, BUFSIZE)) > 0) {
		char * left = buf;
		size_t num_left = num_read;

		while (num_left > 0) {
			int num_written;

			// see if the buffert starts with any zeros
			num_skip = skip_zeros(&left, &num_left);
			if (num_skip) {
				// skip zeros
				if (lseek(dest_fd, num_skip, SEEK_CUR) == -1) {
					goto fail;
				}
			}

			num_written = write_nonzeros(dest_fd, &left, &num_left);
			if (num_written < 0) {
				// writing failed
				goto fail;
			} else if (num_written > 0) {
				// wrote some characters
				num_skip = 0;
			}
		}
	}

	// check if finished because read failed
	if (num_read == -1) {
		goto fail;
	}

	// need to skip to the end?
	if (num_skip) {
		if (lseek(dest_fd, -1, SEEK_CUR) == -1) {
			goto fail;
		}
		// write a zero at the last position to skip the remaining characters
		if (write(dest_fd, "\0", 1) != 1) {
			goto fail;
		}
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

