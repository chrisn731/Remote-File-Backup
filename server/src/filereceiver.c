#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../include/filereceiver.h"
#include "../include/helper.h"
#include "../include/main.h"

#define ERR_EXIT(x) 		\
	do { 			\
		die(x); 	\
		break; 		\
	} while (0)

enum {
	R_ACTION = 0,
	R_FNAME,
	R_FMODE,
	R_FCONT,
	R_COUNT,
};

static void read_data(int sockfd, void *ptr, size_t amt, int op)
{
	int rc;
	char *data = ptr;

	while (amt > 0) {
		rc = read(sockfd, data, amt);
		if (rc < 0) {
			switch (op) {
			case R_ACTION:
				ERR_EXIT("Error receiving action");
			case R_FNAME:
				ERR_EXIT("Error receiving filename");
			case R_FMODE:
				ERR_EXIT("Error receiving filemode");
			case R_FCONT:
				ERR_EXIT("Error receiving filecontent");
			case R_COUNT:
				ERR_EXIT("Error receiving file count");
			default:
				ERR_EXIT("Unknown error while reading data");
			}
		}
		data += rc;
		amt -= rc;
	}
}

void receive_action(int sockfd, char *ft)
{
	read_data(sockfd, ft, sizeof(*ft), R_ACTION);
}

/*
 * The buffer passed into this function will ALWAYS
 * point to a size of STD_BUFF_SZ.
 */
void receive_filename(int sockfd, char *buffer)
{
	memset(buffer, 0, STD_BUFF_SZ);
	read_data(sockfd, buffer, STD_BUFF_SZ, R_FNAME);
}

void receive_filemode(int sockfd, struct stat *st)
{
	int32_t conv;
	read_data(sockfd, &conv, sizeof(int32_t), R_FMODE);
	st->st_mode = ntohl(conv);
}

void receive_filecontent(int sockfd, const char *filename, struct stat *st)
{
	FILE *fp;
	int readamt;
	int32_t conv;
	char buffer[STD_BUFF_SZ];

	if (!(fp = fopen(filename, "w")))
		die("Error opening %s to write to", filename);

	do {
		memset(buffer, 0, STD_BUFF_SZ);

		read_data(sockfd, &conv, sizeof(int32_t), R_FCONT);

		readamt = ntohl(conv);
		read_data(sockfd, buffer, readamt, R_FCONT);

		fwrite(buffer, sizeof(char), readamt, fp);
	} while (readamt == STD_BUFF_SZ);
	if (fclose(fp))
		die("Error closing '%s'", filename);
	chmod(filename, st->st_mode);
}

void receive_numoffiles(int sockfd, int *total)
{
	int32_t conv;
	read_data(sockfd, &conv, sizeof(int32_t), R_COUNT);
	*total = ntohl(conv);
}
