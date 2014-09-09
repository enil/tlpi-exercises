#include "sg.h"
#include "opts.h"
#include "common.h"
// for perror
#include <stdio.h>
// for exit, EXIT_SUCCESS, EXIT_FAILURE, NULL
#include <stdlib.h>
// for open, O_RDONLY
#include <fcntl.h>
// for close
#include <unistd.h>
// for iovec
#include <sys/uio.h>

int main(int argc, char * argv[])
{
	struct sg_opts options;
	int fd = -1;
	char ** buffers = NULL;
	struct iovec * iov = NULL;

	if (read_opts(argc, argv, &options) == -1) {
		exit(EXIT_FAILURE);
	}

	if ((fd = open(options.filename, O_RDONLY)) == -1) {
		perror("Opening file");
		goto fail;
	}

	if ((buffers = allocate_buffers(options.num_bufs, BUF_STR_LEN)) == NULL) {
		perror("Creating buffers");
		goto fail;
	}

	if ((iov = create_iovec(buffers, options.num_bufs, BUF_STR_LEN)) == NULL) {
		perror("Creating iovec");
		goto fail;
	}

	// read the buffers
	if (newreadv(fd, iov, options.num_bufs) == -1) {
		perror("Reading buffers");
		goto fail;
	}

	print_buffers(buffers, options.num_bufs);
	
	free_iovec(iov);
	free_buffers(buffers, options.num_bufs);

	if (close(fd) == -1) {
		perror("Closing file");
		fd = -1;
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	if (iov) {
		free_iovec(iov);
	}
	if (buffers) {
		free_buffers(buffers, options.num_bufs);
	}
	if (fd != -1) {
		close(fd);
	}
	return EXIT_FAILURE;
}
