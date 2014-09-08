#include "newdup.h"

// for fcntl, F_DUPFD
#include <fcntl.h>
// for close
#include <unistd.h>
// for errno
#include <errno.h>

int newdup(int fd)
{
	return fcntl(fd, F_DUPFD, 0);
}

int newdup2(int fd1, int fd2)
{
	if (fd1 == fd2) {
		if (fcntl(fd1, F_GETFL) == -1) {
			// file not open
			return -1;
		}
		return fd1;
	}

	// try to close file open as fd2
	if (close(fd2) == -1 && errno != EBADF) {
		return -1;
	}

	// should yield fd2 (unless race, not thread safe!)
	return fcntl(fd1, F_DUPFD, fd2);
}
