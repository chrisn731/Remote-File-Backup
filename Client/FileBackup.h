#ifndef FILEBACKUP_H_G
#define FILEBACKUP_H_G

#include <sys/stat.h>
#include "../Helper.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void send_filetype(int sockfd, char ft);
void send_filename(int sockfd, char *fn, size_t namelength);
void send_filemode(int sockfd, struct stat *st);
void send_message(int sockfd, char *msg, size_t msgsize);
void send_filecontent(int sockfd, char *filename);





#endif /* FILEBACKUP_H_G */
