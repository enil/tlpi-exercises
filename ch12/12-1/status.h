#ifndef STATUS_H
#define STATUS_H

// for bool
#include <stdbool.h>

/**
 * A callback called for each line in a process status file.
 */
typedef bool (* process_status_line)(const char * label, const char * value, void * data);

/**
 * Parses a process status file.
 */
extern bool parse_status_file(const char * filename, process_status_line callback, void * data);

#endif

