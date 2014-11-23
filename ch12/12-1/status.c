/**
 * @file	status.c
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#include "status.h"

// for fopen, fclose, feof, fgetc, FILE
#include <stdio.h>
// for strndupa
#include <string.h>
// for isspace
#include <ctype.h>
// for bool, true, false
#include <stdbool.h>
// for errno
#include <errno.h>

/** The size of the read buffer. */
#define READ_BUFFER_LEN 256

/**
 * Parses a line in a process status file.
 */
static bool parse_status_line(FILE * file, process_status_line callback, void * data);

bool parse_status_file(const char * filename, process_status_line callback, void * data)
{
	FILE * file;
	bool ret = true;

	if ((file = fopen(filename, "r")) == NULL) {
		ret = false;
		goto out;
	}
	// parse each line
	while (!feof(file)) {
		if (!parse_status_line(file, callback, data)) {
			goto out;
		}
	}

out:
	if (file != NULL && fclose(file) != 0) {
		ret = false;
	}

	return ret;
}

bool parse_status_line(FILE * file, process_status_line callback, void * data)
{
	char buf[READ_BUFFER_LEN];
	char * label, * value;
	// the position in the buffer
	int buf_pos = 0;
	// the next char
	int ch;

	// parse the label
	while ((ch = fgetc(file)) != EOF && (ch != ':' && ch != '\n')) {
		if (buf_pos == READ_BUFFER_LEN) {
			// input too large
			return false;
		}
		buf[buf_pos++] = ch;
	}

	if (ch != ':') {
		// label should be followed by colon
		return false;
	}

	// store the label
	if ((label = strndupa(buf, buf_pos)) == NULL) {
		return false;
	}

	// skip past any spaces
	while ((ch = fgetc(file)) != EOF && isspace(ch));

	// value starts here
	ungetc(ch, file);
	buf_pos = 0;

	// parse the value
	while ((ch = fgetc(file)) != EOF && (ch != '\n')) {
		if (buf_pos == READ_BUFFER_LEN) {
			// input too large
			return false;
		}
		buf[buf_pos++] = ch;
	}

	// store the value
	if ((value = strndupa(buf, buf_pos)) == NULL) {
		return false;
	}

	// process the label and value in the callback
	return callback(label, value, data);
}

