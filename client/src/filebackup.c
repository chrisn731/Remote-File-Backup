#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/filebackup.h"
#include "../include/helper.h"
#include "../include/progress.h"

/* Just so compiler does not complain about switch */
#define FATAL(x) 	\
	do { 		\
		die(x); \
		break; 	\
	} while (0)

enum {
	S_ACTN = 0,
	S_FNAME,
	S_FMODE,
	S_MESSG,
	S_FCONT,
	S_COUNT,
};

/* Send data through socket. */
static void send_data(int sockfd, const void *data, size_t amt, int op)
{
	int rc;
	const char *byte = data;

	while (amt) {
		rc = write(sockfd, byte, amt);
		if (rc < 0) {
			switch (op) {
			case S_ACTN:
				FATAL("Error while sending action");
			case S_FNAME:
				FATAL("Error while sending filename");
			case S_FMODE:
				FATAL("Error while sending filemode");
			case S_MESSG:
				FATAL("Error while sending message");
			case S_FCONT:
				FATAL("Error while sending file content");
			case S_COUNT:
				FATAL("Error while sending file count");
			default:
				FATAL("Fatal unknown error");
			}
		}
		byte += rc;
		amt -= rc;
	}
}

/* Sends actions to server to perform. */
void send_action(int sockfd, char ft)
{
	send_data(sockfd, &ft, sizeof(ft), S_ACTN);
}

/* Send filename to server */
void send_filename(int sockfd, const char *filename)
{
	char buffer[STD_BUFF_SZ] = {0};
	memcpy(buffer, filename, STD_BUFF_SZ - 1);
	send_data(sockfd, buffer, STD_BUFF_SZ, S_FNAME);
}

/* Send filemode to server */
void send_filemode(int sockfd, struct stat *st)
{
	int32_t mode = htonl(st->st_mode);
	send_data(sockfd, &mode, sizeof(int32_t), S_FMODE);
}

/* Send arbitrary messsage to the server */
void send_message(int sockfd, const char *msg, size_t msgsize)
{
	send_data(sockfd, msg, msgsize, S_MESSG);
}

void send_filecount(int sockfd, unsigned int total)
{
	int32_t conv = htonl(total);
	send_data(sockfd, &conv, sizeof(int32_t), S_COUNT);
}

/* Open a file and send the contents of it to server */
void send_filecontent(int sockfd, const char *filename,
			int num_files_backed, int total_files)
{
	FILE *fp;
	int32_t conv;
	int read;
	char data[STD_BUFF_SZ];

	if (!(fp = fopen(filename, "r")))
		die("Error reading from: %s", filename);

	do {
		memset(data, 0, STD_BUFF_SZ);

		read = fread(data, 1, STD_BUFF_SZ, fp);
		/* Send how much data is to be read */
		conv = htonl(read);
		send_data(sockfd, &conv, sizeof(conv), S_FCONT);

		/* Send the data */
		send_data(sockfd, data, read, S_FCONT);
	} while (read == STD_BUFF_SZ);
	if (fclose(fp))
		die("Error closing '%s'", filename);
	/* Update the progress bar for total files backed */
	non_verbose_progressbar(num_files_backed, total_files);
}

/*
 * Open a file and send its contents to the server.
 * While sending the file, show the user and nice progress bar.
 */
void send_filecontent_verbosely(int sockfd, const char *filename,
				struct stat *st)
{
	FILE *fp;
	long totalread, runningread = 0;
	int32_t conv;
	int read;
	char data[STD_BUFF_SZ];

	if (!(fp = fopen(filename, "r")))
		die("Error reading from: %s", filename);

	totalread = st->st_size;
	do {
		memset(data, 0, STD_BUFF_SZ);

		read = fread(data, 1, STD_BUFF_SZ, fp);
		runningread += read;
		/* Send how much data is to be read */
		conv = htonl(read);
		send_data(sockfd, &conv, sizeof(conv), S_FCONT);

		/* Send the data */
		send_data(sockfd, data, read, S_FCONT);
		/* Update progress bar after our data has been sent */
		verbose_progressbar(filename, runningread, totalread);
	} while (read == STD_BUFF_SZ);
	if (fclose(fp))
		die("Error closing '%s'", filename);
	putchar('\n');
}
