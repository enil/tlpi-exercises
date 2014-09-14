 /**
 * @file	env.h
 * @author	Emil Nilsson
 * @license	MIT
 * @date	2014
 */

#ifndef NEWENV_ENV_H
#define NEWENV_ENV_H

/**
 * Sets the value of an environment variable.
 */
extern int newsetenv(const char * name, const char * value, int overwrite);

/**
 * Unsets an environment variable.
 */
extern int newunsetenv(const char * name);

#endif

