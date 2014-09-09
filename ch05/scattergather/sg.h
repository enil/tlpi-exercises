#ifndef SG_SG_H
#define SG_SG_H

// for ssize_t
#include <stdlib.h>
// for iovec
#include <sys/uio.h>

/**
 * Gather read.
 */
extern ssize_t newreadv(int fd, const struct iovec * iov, int iovcnt);

#endif

