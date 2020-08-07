#ifndef HELPER_H_G
#define HELPER_H_G

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define STD_BUFF_SZ 512

void die(const char *err, ...);
int strcmp(const char *, const char *);
void zerobuf(char *buf, size_t bufsize);


#endif /* HELPER_H_G */
