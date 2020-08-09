#include "FileReciever.h"





void recieve_filetype(int sockfd, char *ft)
{
	int rc, left;
	char *type = ft;

	left = sizeof(*type);
	do {
		rc = read(sockfd, type, left);

		if (rc < 0)
			die("Error while reading filetype");

		type += rc;
		left -= rc;
	} while (left > 0);

}

void recieve_filename(int sockfd)
{
	int rc, left;
	char buffer[STD_BUFF_SZ];
	char *data = buffer;

	zerobuf(buffer, STD_BUFF_SZ);
	left = STD_BUFF_SZ;

	do {
		rc = read(sockfd, data, left);

		if (rc < 0)
			die("Error reading filename");

		data += rc;
		left -= rc;
	} while (left > 0);

}

void recieve_filemode(int sockfd)
{
	int rc, left;
	int32_t conv;
	char *data = (char *) &conv;

	left = sizeof(mode_t);
	do {
		rc = read(sockfd, data, left);

		if (rc < 0)
			die("Error recieving filemode");

		data += rc;
		left -= rc;
	} while (left > 0);

	conv = ntohl(conv);
}

void recieve_filecontent(int sockfd)
{
	char buffer[STD_BUFF_SZ];
}
