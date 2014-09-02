/**
 * @file	tee.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "opts.h"
// for fprintf, stdin, stderr
#include <stdio.h>
// for open, close, O_WRONLY, O_CREAT, O_APPEND, O_TRUNC
#include <fcntl.h>
// for read, STDIN_FILENO
#include <unistd.h>
// for S_IRUSR, S_IWUSER, S_IRGRP, S_IWGRP
#include <sys/stat.h>
// for exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for errno
#include <errno.h>

/** Size of the read buffer. */
#define BUFSIZE 1024

/**
 * Prints the latest error and quit.
 */
void __attribute__((__noreturn__)) error()
{
	if (errno) {
		perror("tee");
	} else {
		fprintf(stderr, "Unknown error\n");
	}
	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
	int src_fd = -1, dest_fd = -1;
	int num_read;
	char buf[BUFSIZE];
	struct tee_opts opts;

	opts = read_opts(argc, argv);

	src_fd = STDIN_FILENO;
	dest_fd = open(
			opts.filename,
			O_WRONLY | O_CREAT | (opts.append ? O_APPEND : O_TRUNC),
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (dest_fd == -1) {
		goto fail;
	}

	while ((num_read = read(src_fd, buf, BUFSIZE-1)) > 0) {
		// write to destination file
		if (write(dest_fd, buf, num_read) != num_read) {
			goto fail;
		}

		// insert missing end-of-string
		buf[num_read] = '\0';
		printf("%s", buf);
	}

	if (num_read == -1) {
		goto fail;
	}

	if (close(dest_fd)) {
		// couldn't close file, simply mark it as closed
		dest_fd = -1;
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	if (dest_fd != -1) {
		close(dest_fd);
	}
	error();
}

