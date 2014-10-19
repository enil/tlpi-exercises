#include "newalloc.h"
// for printf, fprintf, scanf, stderr
#include <stdio.h>
// for strtol, free, NULL, EXIT_SUCCESS
#include <stdlib.h>
// for intptr_t
#include <stdint.h>
// for sbrk
#include <unistd.h>
// for strcasecmp
#include <strings.h>
// for LONG_MIN, LONG_MAX
#include <limits.h>
// for bool, true, false
#include <stdbool.h>

/**
 * A command entered by the user.
 */
typedef enum {
	INVALID_COMMAND,
	QUIT_COMMAND,
	ALLOC_COMMAND,
	FREE_COMMAND,
	LIST_COMMAND,
	DETAILED_LIST_COMMAND,
	BRK_COMMAND,
	HELP_COMMAND
} command;

/**
 * A function for a command.
 */
typedef bool (* command_func)(void);

/**
 * Description of a command.
 */
typedef struct {
	command type;
	command_func func;
	char * name;
	char * short_name;
	char * description;
} command_description;

static bool alloc_command(void);
static bool free_command(void);
static bool list_command(void);
static bool detailed_list_command(void);
static bool brk_command(void);
static bool help_command(void);

/**
 * Descriptions for the commands.
 */
static command_description command_descriptions[] = {
	{QUIT_COMMAND,			NULL,					"quit",				"q",	"Quits the program."},
	{ALLOC_COMMAND,			alloc_command,			"alloc [size]",		"a",	"Allocates memory of the specified size."},
	{FREE_COMMAND,			free_command,			"free [address]",	"f",	"Frees the specified memory."},
	{LIST_COMMAND,			list_command,			"list",				"l",	"Displays a list of the allocations."},
	{DETAILED_LIST_COMMAND,	detailed_list_command,	"dlist",			"d",	"Displays a detailed list of the allocations."},
	{BRK_COMMAND,			brk_command,			"brk",				"b",	"Displays the program break."},
	{HELP_COMMAND,			help_command,			"help",				"h",	"Shows usage of all commands."},
	{INVALID_COMMAND,		NULL,					NULL,				NULL,	NULL}
};

#define EACH_COMMAND(command) for (command_description * command = command_descriptions; command->name; ++command)

/**
 * Prompts the user to enter a command.
 */
static command prompt_command(void)
{
	char * buffer;
	command command;

	do {
		// prompt
		printf("> ");

		// WARNING: not protected against overflow
		scanf("%ms", &buffer);

		// find a matching command
		command = INVALID_COMMAND;
		EACH_COMMAND(description) {
			// check against name and short name
			if (strcasecmp(buffer, description->name) == 0
					|| (description->short_name && strcasecmp(buffer, description->short_name) == 0)) {
				command = description->type;
				break;
			}
		}

		if (command == INVALID_COMMAND) {
			fprintf(stderr, "Invalid command, enter \"help\" to display all commands\n");
		}

		free(buffer);
	} while (command == INVALID_COMMAND);

	return command;
}

/**
 * Gets the command function for a command.
 */
static command_func get_command_func(command command)
{
	EACH_COMMAND(description) {
		if (description->type == command) {
			return description->func;
		}
	}
	// not found
	return NULL;
}

/**
 * Reads a long value.
 */
static bool read_long(long * value, int base)
{
	bool result = false;
	char * buffer;
	char * end;
	long converted;

	scanf("%ms", &buffer);
	if (buffer == NULL) {
		goto fail;
	}
	converted = strtol(buffer, &end, base);
	if (converted  == LONG_MIN || converted == LONG_MAX || *end != '\0') {
		goto fail;
	}

	*value = converted;
	result = true;

fail:
	if (buffer != NULL) {
		free(buffer);
	}
	return result;
}

bool alloc_command(void)
{
	long size;

	if (!read_long(&size, 0)) {
		fprintf(stderr, "Invalid size\n");
		return false;
	}

	void * mem = na_malloc((size_t)size);
	printf("Allocated memory at 0x%08lx\n", (intptr_t)mem);
}

bool free_command(void)
{
	uintptr_t address;

	if (!read_long(&address, 16)) {
		fprintf(stderr, "Invalid address\n");
		return false;
	}

	na_free((void *)address);
	printf("Free memory at 0x%08lx\n", address);
}

bool list_command(void)
{
	na_list_nodes();
	return true;
}

bool detailed_list_command(void)
{
	na_print_nodes();
	return true;
}

bool brk_command(void)
{
	printf("brk: 0x%08lx\n", (uintptr_t)sbrk(0));
	return true;
}

bool help_command(void)
{
	printf("Available commands\n");
	EACH_COMMAND(description) {
		printf("%-16s  %1s  %s\n",
				description->name,
				(description->short_name ? description->short_name : ""),
				description->description);
	}
	return true;
}

int main(int argc, char * argv[])
{
	command command;

	printf("Type \"help\" to list available commands\n");
	while ((command = prompt_command()) != QUIT_COMMAND) {
		command_func func = get_command_func(command);
		if (func == NULL) {
			fprintf(stderr, "Unknown command, try again\n");
		} else {
			// execute the command function
			func();
		}
	}
	printf("bye!\n");
	return EXIT_SUCCESS;
}

