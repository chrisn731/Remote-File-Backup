#ifndef HELPER_H_G
#define HELPER_H_G

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

/*
 * The standard buffer size to use across all programs.
 * This helps regulate how much data should be read and wrote
 * from client to socket to server.
 * If more data throughput is needed,
 * this is the only value needed to be changed.
 */
#define STD_BUFF_SZ 4096

/* Port to be used for Client and Server Socket. */
#define PORT 8080

extern void die(const char *err, ...);
extern int strcmp(const char *, const char *);
extern void zerobuf(char *buf, size_t bufsize);
extern void v_log(const char *msg, ...);


#endif /* HELPER_H_G */
