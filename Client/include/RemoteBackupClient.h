#ifndef _RMBC_H_
#define _RMBC_H_
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>


static void backup_dir(int sockfd, const char *path);
static void backup_file(const char *filename, const int sockfd);
static void begin_backup(int sockfd, const char *path);
static int dont_skip(const char *filename);
static void print_usage(void);
static int open_sock(unsigned int port, const char *ip);


#endif /*_RMBC_H_ */
