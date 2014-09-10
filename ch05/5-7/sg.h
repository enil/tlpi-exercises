 /**
 * @file	sg.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef SG_SG_H
#define SG_SG_H

// for ssize_t
#include <stdlib.h>
// for iovec
#include <sys/uio.h>

/**
 * Scatter read.
 */
extern ssize_t newreadv(int fd, const struct iovec * iov, int iovcnt);

/**
 * Gather write.
 */
extern ssize_t newwritev(int fd, const struct iovec * iov, int iovcnt);

#endif

