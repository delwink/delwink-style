/*
 *  style.c - Delwink C coding conventions demo
 *  Copyright (C) 2017 Delwink, LLC
 *
 *  Copying and distribution of this file, with or without modification,
 *  are permitted in any medium without royalty provided the copyright
 *  notice and this notice are preserved.  This file is offered as-is,
 *  without any warranty.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * All source files should have an appropriate license header such as the one
 * above. Multi-line comments look like this.
 */

/* Single-line comments look like this when using C89. */

// Single-line comments look like this when using C99 or later.

#define PROGNAME "c-style"
#define VERSION "20170512"
#define USAGE PROGNAME " v" VERSION "\n"				\
	"Prints Fibonacci sequences\n"					\
	"USAGE: " PROGNAME " [-h] [-v] [-s START] [-e END] [-o FILE]\n" \
	"\n"								\
	"OPTIONS:\n"							\
	"\t-h:\tShows this help and exits.\n"				\
	"\n"								\
	"\t-v:\tShows version info and exits.\n"			\
	"\n"								\
	"\t-s START:\tStarts sequence at START.\n"			\
	"\n"								\
	"\t-e END:\tEnds sequence at END.\n"				\
	"\n"								\
	"\t-o FILE:\tSaves output in FILE.\n"

// Doc comments are doxygen-compliant. See below.

/**
 * @brief Data structure to hold the state of a Fibonacci sequence.
 */
struct fib_state_machine
{ // opening brace always goes on next line
	unsigned int value; // struct members are always on separate lines,
	unsigned int next_value; // even when the same type and adjacent.
	unsigned int index;
}; // don't use typedefs unless you're trying to hide something.

// note: doc comments usually go in headers

/**
 * @brief Initializes a Fibonacci state machine.
 * @param self The machine to be initialized.
 */
static void // function return types stay on their own line, even in headers
fib_init(struct fib_state_machine *self)
{
	/*
	 * Indentation is one tab character everywhere. Don't let lines go
	 * longer than 80 columns. Tabs count as 8 columns.
	 */

	self->value = 0;
	self->next_value = 1;
	self->index = 0;
}

/**
 * @brief Advances a Fibonacci sequence to its next value.
 * @param self The state of the sequence.
 */
static void
fib_advance(struct fib_state_machine *self)
{
	unsigned int temp = self->value + self->next_value;

	self->value = self->next_value;
	self->next_value = temp;
	++self->index; // use prefix when only incrementing/decrementing
}

/**
 * @brief Prints a Fibonacci sequence.
 * @param start The starting index.
 * @param end The ending index (exclusive).
 * @param fmt The print format (MUST contain only one %u for insertion).
 * @param dest The destination of the printed sequence.
 */
static void
print_sequence(unsigned int start, unsigned int end,
	/*
	 * On long parameter lists, indent like so. Use `const` qualifier when
	 * appropriate.
	 */
	const char *fmt, FILE *dest)
{
	if (end <= start)
		return; // fail quickly

	if (!fmt) // fine to use ! to check for NULL pointer values
		fmt = "%u\n"; // no braces needed on single statement

	if (!dest)
		dest = stdout;

	/*
	 * Create variables where it's appropriate. Don't constrain yourself
	 * to putting them at the top of functions, unless using C89.
	 *
	 * Also, let the compiler handle memory allocation when you don't need
	 * dynamic allocation and you're not taking up a lot of space. Use
	 * your own judgment for determining this, but stacks are pretty big
	 * on modern hardware.
	 */
	struct fib_state_machine fib;
	fib_init(&fib);

	while (fib.index < start)
		fib_advance(&fib);

	do
	{
		fprintf(dest, fmt, fib.value);
		fib_advance(&fib);
	} while (fib.index < end);
}

/**
 * @brief Prints usage info and exits.
 * @param rc Exit code to be used.
 */
static void
usage(int rc)
{
	fprintf(stderr, USAGE);
	exit(rc);
}

/**
 * @brief Converts a string to an index or fails.
 * @param src The source string to be converted.
 * @param lower The lower bound of the index.
 * @param upper The upper bound of the index (inclusive).
 * @param arg The argument for this index.
 * @return The specified index, if it was valid.
 */
static unsigned int
str_to_index(const char *src, int lower, int upper, int arg)
{
	int temp;

	int rc = sscanf(src, "%d", &temp);
	if (rc != 1)
	{
		fprintf(stderr, "Argument to -%c must be a number\n", arg);
		exit(1);
	}

	if (temp < lower || temp > upper)
	{
		// don't chop string literals that go over 80 columns
		fprintf(stderr,
			PROGNAME ": argument to -%c must be between %d and %d\n",
			arg, lower, upper);
		exit(1);
	}

	return (unsigned int) temp;
}

int // main ALWAYS returns int
main(int argc, char *argv[])
{
	FILE *outfile = stdout;
	int c;
	unsigned int start = 0, end = 10;

	// use POSIX getopt
	while ((c = getopt(argc, argv, ":e:ho:s:v")) != -1)
	{
		switch (c)
		{
		case 'e':
			end = str_to_index(optarg, 0, 48, c);
			break;

		case 'h':
			usage(0);

		case 'o':
			outfile = fopen(optarg, "w");
			if (!outfile)
			{
				fprintf(stderr,
					PROGNAME ": could not open %s: %s\n",
					optarg, strerror(errno));
				return 1;
			}
			break;

		case 's':
			start = str_to_index(optarg, 0, 48, c);
			break;

		case 'v':
			fprintf(stderr, PROGNAME " v" VERSION "\n");
			return 0;

		case ':':
			fprintf(stderr,
				PROGNAME ": option %c requires an argument\n",
				optopt);
			// SPILLS OVER!
		case '?':
			usage(1);
		}
	}

	print_sequence(start, end, NULL, outfile);

	if (outfile != stdout)
		fclose(outfile);

	return 0;
}
