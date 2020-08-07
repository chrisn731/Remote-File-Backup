#ifndef _RMBC_H_
#define _RMBC_H_
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>


static int open_sock(unsigned int port, const char *ip);
static void die(const char *err, ...);
static int strcmp(const char *, const char *);
static void begin_backup(int sockfd);
static void backup_file(const char *filename, const int sockfd);
static int is_file_to_skip(const char *filename);


#endif /*_RMBC_H_ */
