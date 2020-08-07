#include "FileBackup.h"

#define EOFSIZE 6
#define EOSSIZE 6
#define EOFMSG "~EOF~"
#define EOSMSG "~EOS~"

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

void send_filename(int sockfd, char *filename, size_t namelength)
{
	int rc, left;
	char *data = filename;

	left = namelength;

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
	int32_t conv;
	char *data = (char *) &conv;
	int rc, left;

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

void send_message(int sockfd, char *msg, size_t msgsize)
{
	int rc, left;
	char *data = msg;

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

void send_filecontent(int sockfd, char *filename)
{
	FILE *fp = fopen(filename, "r");
	char data;
	char *byte;
	int rc, left;

	if (!fp)
		die("Error reading from: %s", filename);


	while ((data = fgetc(fp)) != EOF) {
		left = sizeof(data);
		do {
			rc = write(sockfd, &data, left);

			if (rc < 0) {
				die("Error sending filecontent of %s", filename);
			} else {
				data += rc;
				left -= rc;
			}
		} while (left > 0);
	}

	send_message(sockfd, EOFMSG, EOFSIZE);
}
