#include "FileBackup.h"


void send_filetype(int sockfd, char ft)
{
	int rc, left;
	char *type = &ft;

	left = sizeof(ft);
	do {
		rc = write(sockfd, type, left);

		if (rc < 0) {
			die("Error while sending filetype");
		} else {
			type += rc;
			left -= rc;
		}

	} while (left > 0);
}

void send_filename(int sockfd, const char *filename, size_t namelength)
{
	int rc, left;
	char buffer[STD_BUFF_SZ];
	char *data = buffer;

	zerobuf(buffer, STD_BUFF_SZ);
	sprintf(buffer, "%s", filename);

	left = STD_BUFF_SZ;
	do {
		rc = write(sockfd, data, left);

		if (rc < 0) {
			die("Error sending filename for %s", filename);
		} else {
			data += rc;
			left -= rc;
		}
	} while (left > 0);
}

void send_filemode(int sockfd, struct stat *st)
{
	int rc, left;
	int32_t conv = htonl(st->st_mode);
	char *data = (char *) &conv;

	left = sizeof(st->st_mode);
	do {
		rc = write(sockfd, data, left);

		if (rc < 0) {
			die("Error while sending filemode.");
		} else {
			data += rc;
			left -= rc;
		}
	} while (left > 0);

}

void send_message(int sockfd, const char *msg, size_t msgsize)
{
	int rc, left;
	const char *data = msg;

	left = msgsize;
	do {
		rc = write(sockfd, data, left);

		if (rc < 0) {
			die("Error sending msg: %s", msg);
		} else {
			data += rc;
			left -= rc;
		}
	} while (left > 0);
}

void send_filecontent(int sockfd, const char *filename)
{
	FILE *fp = fopen(filename, "r");
	char data;
	char *ptr;
	int rc, left;

	if (!fp)
		die("Error reading from: %s", filename);


	while ((data = fgetc(fp)) != EOF) {
		left = sizeof(data);
		ptr = &data;
		do {
			rc = write(sockfd, ptr, left);

			if (rc < 0) {
				die("Error sending filecontent of %s", filename);
			} else {
				ptr += rc;
				left -= rc;
			}
		} while (left > 0);
	}

	send_message(sockfd, EOFMSG, EOFSIZE);
	fclose(fp);
}
