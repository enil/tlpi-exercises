 /**
 * @file	common.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef SG_COMMON_H
#define SG_COMMON_H

// for size_t
#include <stdlib.h>
// for iovec
#include <sys/uio.h>

/** Size of the strings in the buffers. */
#define BUF_STR_LEN 16

/**
 * Allocates a vector of string buffers.
 */
extern char ** allocate_buffers(int num_bufs, size_t size);

/**
 * Frees a vector of string buffers.
 */
void free_buffers(char ** buffers, int num_bufs);

/**
 * Fills a vector of string buffers with strings.
 */
extern void fill_buffers(char ** buffers, int num_bufs, size_t size, char * str);

/**
 * Prints the contents of a vector of string buffers.
 */
extern void print_buffers(char ** buffers, int num_bufs);

/**
 * Creates an iovec for a vector of string buffers.
 */
extern struct iovec * create_iovec(char ** buffers, int num_bufs, size_t size);

/**
 * Frees an iovec.
 */
extern void free_iovec(struct iovec * iov);

#endif

