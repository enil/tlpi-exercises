#ifndef NEWDUP_NEWDUP_H
#define NEWDUP_NEWDUP_H

/**
 * Reimplementation of dup using fcntl.
 */
extern int newdup(int fd);

/**
 * Reimplementation of dup2 using fcntl.
 */
extern int newdup2(int fd1, int fd2);

#endif

