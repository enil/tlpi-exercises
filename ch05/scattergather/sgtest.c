#include "sg.h"
// for open, O_RDONLY
#include <fcntl.h>
// for close
#include <unistd.h>
// for exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for printf, fprintf, perror, stderr, size_t, ssize_t
#include <stdio.h>
// for memset
#include <string.h>
// for iovec
#include <sys/uio.h>
// for va_start, va_list
#include <stdarg.h>

/** Size of the string buffers. */
#define STR_BUF_SIZE 8

/** Size of the iovec. */
#define IOVEC_SIZE 4

/**
 * Prints usage.
 */
static void usage(void)
{
	fprintf(stderr, "Usage: sgtest filename\n");
}

/**
 * Clears buffers.
 */
static void clear_buffers(int count, size_t size, ...)
{
	va_list args;

	va_start(args, size);

	for (int index = 0; index < count; ++index) {
		char * buf = va_arg(args, char *);

		memset(buf, '?', size-1);
		buf[size-1] = '\0';
	}

	va_end(args);
}

int main(int argc, char * argv[])
{
	int fd = -1;
	char buf1[STR_BUF_SIZE], buf2[STR_BUF_SIZE], buf3[STR_BUF_SIZE], buf4[STR_BUF_SIZE];
	// iovec mapping to all of the buffers
	struct iovec iov[IOVEC_SIZE] = {
		{ .iov_base = buf1, .iov_len = STR_BUF_SIZE },
		{ .iov_base = buf2, .iov_len = STR_BUF_SIZE },
		{ .iov_base = buf3, .iov_len = STR_BUF_SIZE },
		{ .iov_base = buf4, .iov_len = STR_BUF_SIZE }
	};
	char * filename;
	ssize_t bytes_read;

	if (argc != 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	if ((fd = open(filename, O_RDONLY)) == -1) {
		perror("Opening file");
		goto fail;
	}

	clear_buffers(4, STR_BUF_SIZE, buf1, buf2, buf3, buf4);

	if ((bytes_read = newreadv(fd, iov, IOVEC_SIZE)) == -1) {
		perror("Reading file");
		goto fail;
	}

	printf("bytes read: %lld\n", (long long)bytes_read);

	// print content of all buffers
	printf("buf1: %s\n", buf1);
	printf("buf2: %s\n", buf2);
	printf("buf3: %s\n", buf3);
	printf("buf4: %s\n", buf4);

	if (close(fd) == -1) {
		perror("Closing file");
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
