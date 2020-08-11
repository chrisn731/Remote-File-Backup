#ifndef FILERECIEVER_H_G
#define FILERECIEVER_H_G

#include "../../Shared/Helper.h"
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern int verbose;

extern void decipher_message(int sockfd);
extern void recieve_filecontent(int sockfd, const char *filename, struct stat *);
extern void recieve_filemode(int sockfd, struct stat *st);
extern void recieve_filename(int sockfd, char *buffer);
extern void recieve_filetype(int sockfd, char *ft);

#endif /* FILERECIEVER_H_G */
