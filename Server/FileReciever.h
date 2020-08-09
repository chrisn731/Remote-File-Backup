#ifndef FILERECIEVER_H_G
#define FILERECIEVER_H_G

#include "../Helper.h"
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void recieve_filetype(int sockfd, char *ft);
void recieve_filename(int sockfd);
void recieve_filemode(int sockfd);
void decipher_message(int sockfd);
void recieve_filecontent(int sockfd);




#endif /* FILERECIEVER_H_G */
