#ifndef FILEMAKER_H_G
#define FILEMKAER_H_G
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>

static void getmode(struct stat *);
static void fill_file_contents(FILE *fp);
int make_dir(int connfd, char *filename);
int make_file(int connfd);


#endif /* FILEMKAER_H_G */
