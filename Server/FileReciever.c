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

/*
 * The buffer passed into this function will ALWAYS
 * point to a size of STD_BUFF_SZ.
 */
void recieve_filename(int sockfd, char *buffer)
{
	int rc, left;
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

void recieve_filemode(int sockfd, struct stat *st)
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

	st->st_mode = ntohl(conv);
}

void recieve_filecontent(int sockfd, const char *filename, struct stat *st)
{
	char buffer[STD_BUFF_SZ];
	FILE *fp;
	int rc, left, readamt;
	int32_t conv;
	char *ptr;

	fp = fopen(filename, "w");
	if (!fp)
		die("Error opening %s to write to", filename);

	do {
		zerobuf(buffer, STD_BUFF_SZ);
		ptr = (char *) &conv;
		left = sizeof(conv);
		do {
			rc = read(sockfd, ptr, left);

			if (rc < 0)
				die("Error while reading amount of bytes to read");

			ptr += rc;
			left -= rc;
		} while (left > 0);

		readamt = ntohl(conv);
		left = readamt;
		ptr = buffer;

		do {
			rc = read(sockfd, ptr, left);

			if (rc < 0)
				die("Error while reading %s content", filename);

			ptr += rc;
			left -= rc;
		} while (left > 0);

		fwrite(buffer, 1, readamt, fp);
	} while (readamt == STD_BUFF_SZ);

	fclose(fp);
	chmod(filename, st->st_mode);
}
