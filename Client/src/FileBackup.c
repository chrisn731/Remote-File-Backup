#include "../include/FileBackup.h"


static void send_data(int sockfd, void *data, size_t amt, int op)
{
	int rc;
	char *byte = data;

	if (!amt)
		return;

	do {
		rc = write(sockfd, byte, amt);

		if (rc < 0) {
			switch (op) {
			case 1:
			case 2:
			case 3:
			case 4:
			default:
				die("Error");
			}
		}

		byte += rc;
		amt -= rc;

	} while (amt > 0);
}

void send_filetype(int sockfd, char ft)
{
	send_data(sockfd, &ft, sizeof(ft), 1);
}

void send_filename(int sockfd, const char *filename)
{
	char buffer[STD_BUFF_SZ];

	zerobuf(buffer, STD_BUFF_SZ);
	sprintf(buffer, "%s", filename);
	send_data(sockfd, (void *) buffer, STD_BUFF_SZ, 2);
}

void send_filemode(int sockfd, struct stat *st)
{
	int32_t conv;

	conv = htonl(st->st_mode);
	send_data(sockfd, &conv, sizeof(int32_t), 3);
}

void send_message(int sockfd, const char *msg, size_t msgsize)
{
	send_data(sockfd, (void *) msg, msgsize, 0);
}

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

		conv = htonl(read);

		/* Send how much data is to be read */
		send_data(sockfd, &conv, sizeof(conv), 4);

		/* Send the data */
		send_data(sockfd, data, read, 4);

	} while (read == STD_BUFF_SZ);

	fclose(fp);
}
