 /**
 * @file	opts.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef SG_OPTS_H
#define SG_OPTS_H

struct sg_opts {
	/** The number of buffers. */
	int num_bufs;

	/** The filename of the file to read from/write to. */
	char * filename;
};

/**
 * Reads options.
 */
extern int read_opts(int argc, char * argv[], struct sg_opts * options);

#endif

