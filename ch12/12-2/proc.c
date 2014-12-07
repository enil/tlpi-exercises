#include "proc.h"

// for fgets
#include <stdio.h>
// for strchr, strcpy, strdup, memset
#include <string.h>
// for isspace
#include <ctype.h>
// for malloc, free, strtol
#include <stdlib.h>
// for LONG_MIN, LONG_MAX
#include <limits.h>
// for errno
#include <errno.h>

/** The size of the line buffer for process files. */
#define PTREE_PROC_LINE_SIZE 1024

#define MIN(x, y) ((x < y) ? (x) : (y))

/**
 * The components of a line in a proc file.
 */
typedef struct proc_line_t {
	char * label;
	char * value;
} proc_line_t;

/**
 * Creates a process information structure.
 */
static proc_t * alloc_proc(void);

/**
 * Parses the value of a line in a proc file.
 */
static int parse_value(const char * label, const char * value, proc_t * proc);

/**
 * Parses the PPID in a proc file.
 */
static int parse_ppid(const char * value, pid_t * pid);

/**
 * Parses the name in a proc file.
 */
static int parse_name(const char * value, const char ** name);

/**
 * Splits a line in a proc file.
 */
static int split_line(const char * line, proc_line_t * components);

/**
 * Copies text from a string until the occurance of a delimiter.
 */
static ssize_t copy_until(char * dest, const char * src, size_t size, char delim, const char ** end);

/**
 * Skips past spaces at the start of a string.
 */
static size_t skip_spaces(const char * str, const char ** end);

/**
 * Removes any newline character at the end of a string.
 */
static char * strip_newline(char * str);

proc_t * read_proc(FILE * file)
{
	proc_t * proc = NULL;
	char line[PTREE_PROC_LINE_SIZE];
	proc_line_t components;

	if ((proc = alloc_proc()) == NULL) {
		goto fail;
	}

	while ((fgets(line, PTREE_PROC_LINE_SIZE, file))) {
		if (split_line(line, &components) == -1) {
			goto fail;
		}
		if (parse_value(components.label, components.value, proc) == -1) {
			goto fail;
		}
	}

	if (errno) {
		// reading failed
		goto fail;
	}

	return proc;

fail:
	if (proc) {
		free_proc(proc);
	}
	return NULL;
}

proc_t * alloc_proc(void)
{
	proc_t * proc = NULL;

	if ((proc = malloc(sizeof(proc_t))) == NULL) {
		return NULL;
	}
	memset(proc, 0, sizeof(proc_t));
	return proc;
}

void free_proc(proc_t * proc)
{
	if (proc->name != NULL) {
		free(proc);
	}
}

int parse_value(const char * label, const char * value, proc_t * proc)
{
	if (strcmp("Name", label) == 0) {
		return parse_name(value, &proc->name);
	} else if (strcmp("PPid", label) == 0) {
		return parse_ppid(value, &proc->ppid);
	}
	return 0;
}

int parse_ppid(const char * value, pid_t * pid)
{
	long long_val;
	char * end;

	long_val = strtol(value, &end, 10);
	if (*end != '\0' || long_val == LONG_MIN || long_val == LONG_MAX) {
		return -1;
	}

	*pid = (pid_t)long_val;
	return 0;
}

int parse_name(const char * value, const char ** name)
{
	if ((*name = strdup(value)) == NULL) {
		return -1;
	}
	return 0;
}

int split_line(const char * line, proc_line_t * components)
{
	static char label[PTREE_PROC_LINE_SIZE];
	static char value[PTREE_PROC_LINE_SIZE];
	const char * pos = line;

	// copy the label
	if (copy_until(label, pos, PTREE_PROC_LINE_SIZE, ':', &pos) == -1) {
		return -1;
	}

	// skip spaces before the value
	if (skip_spaces(pos, &pos) == -1) {
		return -1;
	}

	// copy the value
	if (copy_until(value, pos, PTREE_PROC_LINE_SIZE, '\0', &pos) == -1) {
		return -1;
	}

	memset(components, 0, sizeof(proc_line_t));
	components->label = label;
	components->value = strip_newline(value);

	return 0;
}

ssize_t copy_until(char * dest, const char * src, size_t size, char delim, const char ** end)
{
	const char * cur = src;
	ssize_t len = 0;

	while (*cur && *cur != delim) {
		cur++;
		len++;
	}

	if (len > size-1 || *cur != delim) {
		return -1;
	}

	*end = cur+1;
	strncpy(dest, src, len);
	dest[len] = '\0';

	return len;
}

size_t skip_spaces(const char * str, const char ** end)
{
	const char * cur = str;
	size_t len = 0;

	while (*cur && isspace(*cur)) {
		cur++;
		len++;
	}

	*end = cur;
	return len;
}

char * strip_newline(char * str)
{
	char * cur = str;
	while (*cur) {
		cur++;
	}

	char * prev = cur-1;
	if (cur != str && *prev == '\n') {
		*prev = '\0';
	}
	return str;
}

