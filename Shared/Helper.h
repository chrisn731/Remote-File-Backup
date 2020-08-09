#ifndef HELPER_H_G
#define HELPER_H_G

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define STD_BUFF_SZ 512

static int verbose = 0;

void die(const char *err, ...);
int strcmp(const char *, const char *);
void zerobuf(char *buf, size_t bufsize);
void v_log(const char *msg, ...);


#endif /* HELPER_H_G */
