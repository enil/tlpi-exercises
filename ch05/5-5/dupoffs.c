 /**
 * @file	dupoffs.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

// for printf, perror
#include <stdio.h>
// for mkstemp, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for close, unlink, dup, lseek, SEEK_SET, SEEK_CUR, SEEK_END
#include <unistd.h>
// for true, false
#include <stdbool.h>
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
 * Check that all file descriptors have the same offset.
 */
static int check_offsets(int fds[], size_t count, off_t expected_offset)
{
	int failed = 0;

	for (int index = 0; index < count; ++index) {
		off_t actual_offset;
		bool valid = true;

		if ((actual_offset = lseek(fds[index], 0, SEEK_CUR)) == -1) {
			return -1;
		}

		// validate the ofset
		valid = (actual_offset == expected_offset);
		if (!valid) {
			failed++;
		}

		// print the offset of the file descriptor
		printf("+ fd %d: offset=%lld [%s]\n", index, (long long)actual_offset, (valid ? "VALID" : "INVALID!"));
	}

	printf("Total: %ld, failed: %d\n\n", count, failed);

	return failed;
}

/**
 * Moves the file descriptor of a file descriptor and check that all move.
 */
static int lseek_and_check(int fds[], size_t count, int selected, off_t offset, int whence)
{
	off_t new_offset;

	// move the offset of the selected file descriptor
   	if ((new_offset = lseek(fds[selected], offset, whence)) == -1) {
		return -1;
	}

	// print information about the seek operation
	printf("lseek(fds[%d], %lld, %s) = %lld\n",
			selected, (long long)offset, whence_to_string(whence), (long long)new_offset);

	// check and print the offsets of all file descriptors
	return check_offsets(fds, count, new_offset);
}

/**
 * Writes to a file descriptor and check that all move.
 */
static int write_and_check(int fds[], size_t count, int selected, const size_t size)
{
	// junk data with the specified size
	char data[size];
	off_t new_offset;

	// write the junk data
	if (write(fds[selected], data, size) != size) {
		return -1;
	}

	// get the new offset of the selected file descriptor
   	if ((new_offset = lseek(fds[selected], 0, SEEK_CUR)) == -1) {
		return -1;
	}

	// print information about the write operation
	printf("write(fds[%d], data, %ld) = %ld\n", selected, size, size);

	// check and print the offsets of all file descriptors
	return check_offsets(fds, count, new_offset);
}

/**
 * Reads from a file descriptor and check that all move.
 */
static int read_and_check(int fds[], size_t count, int selected, const size_t size)
{
	// data buffer with the specified size
	char data[size];
	off_t new_offset;

	// write the data
	if (read(fds[selected], data, size) != size) {
		return errno;
	}

	// get the new offset of the selected file descriptor
   	if ((new_offset = lseek(fds[selected], 0, SEEK_CUR)) == -1) {
		return errno;
	}

	// print information about the write operation
	printf("read(fds[%d], data, %ld) = %ld\n", selected, size, size);

	// check and print the offsets of all file descriptors
	return check_offsets(fds, count, new_offset);
}

int main(int argc, char * argv[])
{
	// a list of file descriptors, index 0 stores the original
	int fds[] = {[0 ... NUM_FDS-1] = -1};

	// create inital file and duplicate file descriptors
	if (setup_fds(fds, NUM_FDS) != 0) {
		perror("Setting up file descriptors");
		goto fail;
	}

	// check offset after calling lseek, write and read for different file descriptors
	if (lseek_and_check(fds, NUM_FDS, 1, 10, SEEK_SET) == -1) {
		perror("Running checks for lseek");
		goto fail;
	}
	if (lseek_and_check(fds, NUM_FDS, 2, -5, SEEK_CUR) == -1) {
		perror("Running checks for lseek");
		goto fail;
	}
	if (write_and_check(fds, NUM_FDS, 3, 20) == -1) {
		perror("Running checks for write");
		goto fail;
	}
	if (lseek_and_check(fds, NUM_FDS, 0, -8, SEEK_END) == -1) {
		perror("Running checks for lseek");
		goto fail;
	}
	if (read_and_check(fds, NUM_FDS, 1, 4) == -1) {
		perror("Running checks for read");
		goto fail;
	}

	// clean up
	close_fds(fds, NUM_FDS);

	return EXIT_SUCCESS;

fail:
	// clean up
	close_fds(fds, NUM_FDS);
	return EXIT_FAILURE;
}
