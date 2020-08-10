#include "../include/FileBackup.h"


void send_data(int sockfd, void *data, size_t amt, int op)
{
	int rc;
	char *byte = data;

	do {
		rc = write(sockfd, data, amt);

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

		data += rc;
		amt -= rc;

	} while (amt > 0);
}

void send_filetype(int sockfd, char ft)
{
	send_data(sockfd, &ft, sizeof(ft), 1);
}

void send_filename(int sockfd, const char *filename, size_t namelength)
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
	char *ptr;
	int32_t conv;
	int rc, read, left;


	fp = fopen(filename, "r");
	if (!fp)
		die("Error reading from: %s", filename);

	do {
		zerobuf(data, STD_BUFF_SZ);
		read = fread(data, 1, STD_BUFF_SZ, fp);

		if (feof(fp))
			read = 0;

		conv = htonl(read);
		ptr = (char *) &conv;

		/* Send how much data is to be read */
		send_data(sockfd, ptr, sizeof(conv), 4);

		if (!read)
			break;

		ptr = data;

		/* Send the data */
		send_data(sockfd, ptr, read, 4);

	} while (read == STD_BUFF_SZ);

	fclose(fp);
}
