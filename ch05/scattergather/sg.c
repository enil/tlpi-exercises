#include "sg.h"

// for malloc, free, NULL
#include <stdlib.h>
// for read
#include <unistd.h>
// for memcpy
#include <string.h>
// for errno, ENOMEM
#include <errno.h>

/** Loops over an iovec array. */
#define EACH_IOVEC(cur, start, len) for (const struct iovec * cur = start, * end = start+len; cur != end; ++cur)

ssize_t newreadv(int fd, const struct iovec * iov, int iovcnt)
{
	size_t buf_size = 0;
	ssize_t bytes_read;
	size_t bytes_left;
	char * buf = NULL;
	char * next;

	// calculate size of the buffer
	EACH_IOVEC(cur, iov, iovcnt) {
		buf_size += cur->iov_len;
	}

	// allocate buffer
	if ((buf = (char *)malloc(sizeof(*buf) * buf_size)) == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	// read the entire buffer
	switch (bytes_read = read(fd, buf, buf_size)) {
		case 0:
			// reached eof
			goto finished;
		case -1:
			// reading failed
			goto fail;
	}

	// start from the beginning of the buffer
	next = buf;
	bytes_left = bytes_read;
	// populate the buffers from iov
	EACH_IOVEC(cur, iov, iovcnt) {
		size_t buf_size = cur->iov_len;

		if (bytes_left == 0) {
			// none left
			break;
		} else if (bytes_left < buf_size) {
			// partial read
			buf_size = bytes_left;
		}
		memcpy(cur->iov_base, next, buf_size);
		next += buf_size;
		bytes_left -= buf_size;
	}

finished:
	free(buf);

	return bytes_read;

fail:
	if (buf) {
		free(buf);
	}
	return -1;
}

ssize_t newwritev(int fd, const struct iovec * iov, int iovcnt)
{
	size_t buf_size = 0;
	ssize_t bytes_written;
	char * buf = NULL;
	char * next;

	// calculate size of the buffer
	EACH_IOVEC(cur, iov, iovcnt) {
		buf_size += cur->iov_len;
	}

	// allocate buffer
	if ((buf = (char *)malloc(sizeof(*buf) * buf_size)) == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	// start from the beginning of the buffer
	next = buf;
	// fill buffer
	EACH_IOVEC(cur, iov, iovcnt) {
		memcpy(next, cur->iov_base, cur->iov_len);
		next += cur->iov_len;
	}

	// write the buffer
	if ((bytes_written = write(fd, buf, buf_size)) == -1) {
		goto fail;
	}

	free(buf);

	return bytes_written;
fail:
	if (buf) {
		free(buf);
	}
	return -1;
}
