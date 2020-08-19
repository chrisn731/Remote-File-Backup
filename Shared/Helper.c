#include "Helper.h"

/* Prints err to stdout and exits the program. */
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

int strcmp(const char *p1, const char *p2)
{
	char c1, c2;

	do {
		c1 = *p1++;
		c2 = *p2++;

		if (c1 == '\0')
			return c2 - c1;

	} while (c1 == c2);

	return c2 - c1;
}

/* Zero out buffer (buf) of a given size (bufsize) */
void zerobuf(char *buf, size_t bufsize)
{
	while (bufsize-- > 0)
		*buf++ = '\0';
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
