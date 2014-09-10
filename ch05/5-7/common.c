 /**
 * @file	common.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "common.h"

// for malloc, free, NULL, size_t
#include <stdlib.h>
// for printf, snprintf
#include <stdio.h>
// for memset
#include <string.h>

char ** allocate_buffers(int num_bufs, size_t size)
{
	char ** buffers = NULL;

	// allocate the list of the string buffers
	if ((buffers = (char **)malloc(sizeof(char *) * num_bufs)) == NULL) {
		goto fail;
	}

	// set all buffer pointers to NULL
	memset(buffers, 0, sizeof(char *) * num_bufs);

	int index;
	for (index = 0; index < num_bufs; ++index) {
		if ((buffers[index] = (char *)malloc(sizeof(char) * size)) == NULL) {
			goto fail;
		}
	}

	return buffers;
fail:
	free_buffers(buffers, num_bufs);
	return NULL;
}

void free_buffers(char ** buffers, int num_bufs)
{
	if (buffers) {
		for (int index = 0; index < num_bufs; ++index) {
			if (buffers[index]) {
				free(buffers[index]);
			}
		}
		free(buffers);
	}
}

void fill_buffers(char ** buffers, int num_bufs, size_t size, char * str)
{
	for (int index = 0; index < num_bufs; ++index) {
		// generate buffer string
		snprintf(buffers[index], size, "%s: %d", str, index);
	}
}

void print_buffers(char ** buffers, int num_bufs)
{
	for (int index = 0; index < num_bufs; ++index) {
		printf("%s\n", buffers[index]);
	}
}

struct iovec * create_iovec(char ** buffers, int num_bufs, size_t size)
{
	struct iovec * iov = NULL;

	if ((iov = (struct iovec *)malloc(sizeof(struct iovec) * num_bufs)) == NULL) {
		return NULL;
	}

	for (int index = 0; index < num_bufs; ++index) {
		iov[index].iov_base = buffers[index];
		iov[index].iov_len = size;
	}

	return iov;
}

void free_iovec(struct iovec * iov)
{
	free(iov);
}

