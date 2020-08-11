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


extern int verbose;

extern void send_data(int sockfd, void *data, size_t size, int op);
extern void send_filetype(int sockfd, char ft);
extern void send_filename(int sockfd, const char *fn);
extern void send_filemode(int sockfd, struct stat *st);
extern void send_message(int sockfd, const char *msg, size_t msgsize);
extern void send_filecontent(int sockfd, const char *filename);


#endif /* FILEBACKUP_H_G */
