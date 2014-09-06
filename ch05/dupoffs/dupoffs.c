// for printf, perror
#include <stdio.h>
// for mkstemp, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for close, unlink, dup, lseek, SEEK_SET, SEEK_CUR, SEEK_END
#include <unistd.h>
// for assert
#include <assert.h>
// for errno
#include <errno.h>

/** The number of file descriptors to create. */
#define NUM_FDS 4

/**
 * Formats a string from the whence argument of lseek.
 */
static char * whence_to_string(int whence)
{
	switch (whence) {
		case SEEK_SET:
			return "SEEK_SET";
		case SEEK_CUR:
			return "SEEK_CUR";
		case SEEK_END:
			return "SEEK_END";
		default:
			return "invalid whence value";
	}
}

/**
 * Creates an inital file descriptor.
 */
static int create_fd(void)
{
	char filename[] = "/tmp/dupoffsXXXXXX";
	int fd = mkstemp(filename);

	if (fd != -1) {
		// delete after closing
		unlink(filename);
	}

	return fd;
}

/**
 * Sets up all file descriptors.
 */
static int setup_fds(int fds[], size_t count)
{
	int error;

	assert(count > 1);

	// create the inital file descriptor
	fds[0] = create_fd();
	if (fds[0] == -1) {
		goto fail;
	}

	// duplicate the rest of the file descriptors
	for (int index = 1; index < count; ++index) {
		if ((fds[index] = dup(fds[0])) == -1) {
			// failed to duplicate file descriptor
			goto fail;
		}
	}

	// success
	return 0;

fail:
	error = errno;

	// close all open files
	for (int index = 0; index < count; ++index) {
		if (fds[index] != -1) {
			close(fds[index]);
		}
	}
	return error;
}

/**
 * Closes all open file descriptors.
 */
static int close_fds(int fds[], size_t count)
{
	for (int index = 0; index < count; ++index) {
		if (fds[index] != -1) {
			if (close(fds[index]) == -1) {
				// could not close file descriptor
				return errno;
			}
		}
	}

	// success
	return 0;
}

/**
 * Moves the file descriptor of a file descriptor and check that all move.
 */
static int move_and_check(int fds[], size_t count, int selected, off_t offset, int whence)
{
	// move the offset of the selected file descriptor
	off_t new_offset = lseek(fds[selected], offset, whence);

	// print information about the seek operation
	printf("lseek(fds[%d], %lld, %s) = %lld\n",
			selected, (long long)offset, whence_to_string(whence), (long long)new_offset);

	for (int index = 0; index < count; ++index) {
		off_t actual_offset;

		// validate the new offset
		if ((actual_offset = lseek(fds[index], 0, SEEK_CUR)) == -1) {
			return errno;
		}
		assert(actual_offset == new_offset);

		printf("fd %d: offset=%lld\n", index, (long long)actual_offset);
	}
	
	// success
	return 0;
}

int main(int argc, char * argv[])
{
	// a list of file descriptors, index 0 stores the original
	int fds[] = {[0 ... NUM_FDS-1] = -1};

	// create inital file and duplicate file descriptors
	if (setup_fds(fds, NUM_FDS) != 0) {
		perror("Setting up file descriptors");
		exit(EXIT_FAILURE);
	}

	move_and_check(fds, NUM_FDS, 1, 10, SEEK_SET);

	// clean up
	close_fds(fds, NUM_FDS);

	return EXIT_SUCCESS;
}
