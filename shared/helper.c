#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "helper.h"

/* Prints err to stderr and exits the program. */
void die(const char *err, ...)
{
	va_list argp;
	va_start(argp, err);
	fputc('\n', stderr);
	vfprintf(stderr, err, argp);
	va_end(argp);
	fputc('\n', stderr);
	exit(1);
}

/* Verbose Logging. */
void v_log(const char *msg, ...)
{
	va_list argp;
	va_start(argp, msg);
	vfprintf(stdout, msg, argp);
	va_end(argp);
	fputc('\n', stdout);
}
