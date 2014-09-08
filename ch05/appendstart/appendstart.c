// for exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// for fprintf, perror, stderr
#include <stdio.h>
// for open, O_WRONLY, O_APPEND
#include <fcntl.h>
// for close, write, lseek, SEEK_SET
#include <unistd.h>
// for strlen
#include <string.h>
// for S_IRUSR, S_IWUSR, S_IRGRP, S_IWGRP
#include <sys/types.h>

/** The file permissions used when creating a new file. */
#define FILE_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

static void usage(void)
{
	fprintf(stderr, "Usage: appendstart filename\n");
}

int main(int argc, char * argv[])
{
	char output[] = "Appended text\n";
	size_t output_len = strlen(output);
	char * filename;
	int fd = -1;

	if (argc != 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	if ((fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMISSIONS)) == -1) {
		perror("Opening file");
		goto fail;
	}

	// seek to the start of the file
	if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("Seeking to start of file");
		goto fail;
	}

	if (write(fd, output, output_len) != output_len) {
		perror("Appending output");
		goto fail;
	}

	if (close(fd) == -1) {
		perror("Closing file");
		goto fail;
	}

	return EXIT_SUCCESS;

fail:
	if (fd != -1) {
		close(fd);
	}
	return EXIT_FAILURE;
}
