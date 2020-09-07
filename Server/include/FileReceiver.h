#ifndef FILERECEIVER_H_G
#define FILERECEIVER_H_G

#include "Helper.h"
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern void receive_action(int sockfd, char *ft);
extern void receive_filecontent(int sockfd, const char *filename, struct stat *);
extern void receive_filemode(int sockfd, struct stat *st);
extern void receive_filename(int sockfd, char *buffer);
extern void receive_numoffiles(int sockfd, int *);

#endif /* FILERECEIVER_H_G */
