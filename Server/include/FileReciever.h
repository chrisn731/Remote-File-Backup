#ifndef FILERECIEVER_H_G
#define FILERECIEVER_H_G

#include "../../Shared/Helper.h"
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void recieve_filetype(int sockfd, char *ft);
void recieve_filename(int sockfd, char *buffer);
void recieve_filemode(int sockfd, struct stat *st);
void decipher_message(int sockfd);
void recieve_filecontent(int sockfd, const char *filename, struct stat *);

#endif /* FILERECIEVER_H_G */
