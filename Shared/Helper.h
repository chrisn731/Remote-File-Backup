#ifndef HELPER_H_G
#define HELPER_H_G

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define STD_BUFF_SZ 512

extern int verbose;

extern void die(const char *err, ...);
extern int strcmp(const char *, const char *);
extern void zerobuf(char *buf, size_t bufsize);
extern void v_log(const char *msg, ...);


#endif /* HELPER_H_G */
