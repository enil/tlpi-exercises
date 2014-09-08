#include "newdup.h"
// for fprintf, snprintf, stderr
#include <stdio.h>
// for exit, perror, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for open
#include <fcntl.h>
// for close
#include <unistd.h>
// for errno
#include <errno.h>

/** The number of file descriptors to create. */
#define NUM_FDS 4

/** File descriptor number for fds[2]. */
#define FDS_2_FD 10

/** The file permissions used when creating a new file. */
#define FILE_PERMISSIONS S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP

/** A message to write to the file. */
#define MESSAGE "Hello from file descriptor %d!\n"

/** The size of the message buffer. */
#define MESSAGE_BUF_LEN 32

/**
 * Prints usage.
 */
static void usage(void)
{
	fprintf(stderr, "Usage: newduptest [file]\n");
}

/**
 * Writes a message to a file.
 */
static int write_message(int fd)
{
	char message_buf[MESSAGE_BUF_LEN];
	int len;

	// create message with the file descriptor
	len = snprintf(message_buf, MESSAGE_BUF_LEN, MESSAGE, fd);

	if (write(fd, message_buf, len) != len) {
		return errno;
	}

	// success
	return 0;
}

/**
 * Closes all files.
 */
static int close_files(int fds[], int num_fds)
{
	for (int index = 0; index < num_fds; ++index) {
		if (fds[index] != -1) {
			if (close(fds[index]) == -1) {
				return errno;
			}
		}
	}

	// success
	return 0;
}

int main(int argc, char * argv[])
{
	char * filename;
	int fds[NUM_FDS] = { [0 ... NUM_FDS-1] = -1 };

	if (argc < 1 || argc > 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	// write to stdout if no file has been specified
	filename = argc == 2 ? argv[1] : "/dev/stdout";

	if ((fds[0] = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS)) == -1) {
		perror("Opening file");
		goto fail;
	}

	// write using the original file descriptor
	if (write_message(fds[0]) != 0) {
		perror("Writing message");
		goto fail;
	}

	// duplicate with newdup and write to it
	if ((fds[1] = newdup(fds[0])) != -1 && write_message(fds[1]) != 0) {
		perror("Duplicating file descriptor with newdup");
		goto fail;
	}

	// duplicate with newdup2 and write to it
	if ((fds[2] = newdup2(fds[0], FDS_2_FD)) != -1 && write_message(fds[2]) != 0) {
		perror("Duplicating file descriptor with newdup2");
		goto fail;
	}

	// duplicate as previously used file descriptor and write to it
	if ((fds[3] = newdup2(fds[0], fds[1])) != -1 && write_message(fds[3]) != 0) {
		perror("Duplicating file descriptor with newdup2");
		goto fail;
	} else {
		// not using this file descriptor any more
		fds[1] = -1;
	}

	if (close_files(fds, NUM_FDS) != 0) {
		perror("Closing files");
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	close_files(fds, NUM_FDS);
	return EXIT_FAILURE;
}
