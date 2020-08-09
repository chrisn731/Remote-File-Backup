#ifndef FILEBACKUP_H_G
#define FILEBACKUP_H_G

#include <sys/stat.h>
#include "../../Shared/Helper.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define EOFSIZE 6
#define EOSSIZE 6
#define EOFMSG "~EOF~"
#define EOSMSG "~EOS~"

void send_data(int sockfd, void *data, size_t size, int op);
void send_filetype(int sockfd, char ft);
void send_filename(int sockfd, const char *fn, size_t namelength);
void send_filemode(int sockfd, struct stat *st);
void send_message(int sockfd, const char *msg, size_t msgsize);
void send_filecontent(int sockfd, const char *filename);





#endif /* FILEBACKUP_H_G */
