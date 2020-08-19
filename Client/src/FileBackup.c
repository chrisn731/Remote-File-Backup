#include "../include/FileBackup.h"

enum operation {
	S_ACTN = 0,
	S_FNAME,
	S_FMODE,
	S_MESSG,
	S_FCONT,
	S_COUNT,
};

/* Send data through socket. */
static void send_data(int sockfd, void *data, size_t amt, enum operation op)
{
	int rc;
	char *byte = data;

	if (!amt)
		return;

	do {
		rc = write(sockfd, byte, amt);
		if (rc < 0) {
			switch (op) {
			case S_ACTN:
				die("Error while sending action");
			case S_FNAME:
				die("Error while sending filename");
			case S_FMODE:
				die("Error while sending filemode");
			case S_MESSG:
				die("Error while sending message");
			case S_FCONT:
				die("Error while sending file content");
			case S_COUNT:
				die("Error while sending file count");
			default:
				die("Fatal unknown error");;
			}
		}
		byte += rc;
		amt -= rc;
	} while (amt > 0);
}

/* Sends actions to server to perform. */
void send_action(int sockfd, char ft)
{
	send_data(sockfd, &ft, sizeof(ft), S_ACTN);
}

/* Send filename to server */
void send_filename(int sockfd, const char *filename)
{
	char buffer[STD_BUFF_SZ];

	zerobuf(buffer, STD_BUFF_SZ);
	sprintf(buffer, "%s", filename);
	send_data(sockfd, (void *) buffer, STD_BUFF_SZ, S_FNAME);
}

/* Send filemode to server */
void send_filemode(int sockfd, struct stat *st)
{
	int32_t conv;

	conv = htonl(st->st_mode);
	send_data(sockfd, &conv, sizeof(int32_t), S_FMODE);
}

/* Send arbitrary messsage to the server */
void send_message(int sockfd, const char *msg, size_t msgsize)
{
	send_data(sockfd, (void *) msg, msgsize, S_MESSG);
}

void send_filecount(int sockfd, unsigned int total)
{
	int32_t conv;

	conv = htonl(total);
	send_data(sockfd, &conv, sizeof(int32_t), S_COUNT);
}

/* Open a file and send the contents of it to server */
void send_filecontent(int sockfd, const char *filename)
{
	FILE *fp;
	char data[STD_BUFF_SZ];
	int32_t conv;
	int read;

	if (!(fp = fopen(filename, "r")))
		die("Error reading from: %s", filename);

	do {
		zerobuf(data, STD_BUFF_SZ);
		read = fread(data, 1, STD_BUFF_SZ, fp);

		/* Send how much data is to be read */
		conv = htonl(read);
		send_data(sockfd, &conv, sizeof(conv), S_FCONT);

		/* Send the data */
		send_data(sockfd, data, read, S_FCONT);

	} while (read == STD_BUFF_SZ);

	fclose(fp);
}

/*
 * Open a file and send its contents to the server.
 * While sending the file, show the user and nice progress bar.
 */
void send_filecontent_verbosely(int sockfd, const char *filename,
				struct stat *st)
{
	FILE *fp;
	char data[STD_BUFF_SZ];
	int32_t conv;
	int read;
	long totalread, runningread = 0;

	if (!(fp = fopen(filename, "r")))
		die("Error reading from: %s", filename);

	totalread = st->st_size;

	do {
		zerobuf(data, STD_BUFF_SZ);
		read = fread(data, 1, STD_BUFF_SZ, fp);
		runningread += read;

		/* Send how much data is to be read */
		conv = htonl(read);
		send_data(sockfd, &conv, sizeof(conv), S_FCONT);

		/* Send the data */
		send_data(sockfd, data, read, S_FCONT);
		verbose_progressbar(filename, runningread, totalread);

	} while (read == STD_BUFF_SZ);
	putchar('\n');

	fclose(fp);
}
