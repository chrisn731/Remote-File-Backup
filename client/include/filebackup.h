#ifndef FILEBACKUP_H_G
#define FILEBACKUP_H_G

#include <sys/stat.h>

extern void send_action(int sockfd, char ft);
extern void send_filecontent(int sockfd, const char *filename, int, int);
extern void send_filecontent_verbosely(int sockfd, const char *fname, struct stat *);
extern void send_filecount(int sockfd, unsigned int total);
extern void send_filemode(int sockfd, struct stat *st);
extern void send_filename(int sockfd, const char *fn);
extern void send_message(int sockfd, const char *msg, size_t msgsize);


#endif /* FILEBACKUP_H_G */
