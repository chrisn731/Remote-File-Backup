#ifndef _RMBC_H_
#define _RMBC_H_
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>


static int open_sock(unsigned int port, const char *ip);
static void begin_backup(int sockfd, const char *path);
static void backup_file(const char *filename, const int sockfd);
static int can_delete(const char *filename);
static void print_usage(void);


#endif /*_RMBC_H_ */
