#ifndef FILEBACKUP_H_G
#define FILEBACKUP_H_G

#include <sys/stat.h>
#include "../../Shared/Helper.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern int verbose;

extern void send_filecontent(int sockfd, const char *filename);
extern void send_filemode(int sockfd, struct stat *st);
extern void send_filename(int sockfd, const char *fn);
extern void send_filetype(int sockfd, char ft);
extern void send_message(int sockfd, const char *msg, size_t msgsize);


#endif /* FILEBACKUP_H_G */
