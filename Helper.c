#include "Helper.h"

void die(const char *err, ...)
{
	va_list argp;
	va_start(argp, err);
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

void zerobuf(char *buf, size_t bufsize)
{
	while (bufsize-- > 0)
		*buf++ = '\0';
}
