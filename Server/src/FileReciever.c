#include "../include/FileReciever.h"

static void read_data(int sockfd, void *ptr, size_t amt, int op)
{
	int rc;
	char *data = ptr;

	if (!amt)
		return;

	do {
		rc = read(sockfd, data, amt);

		if (rc < 0) {
			/*
			 * if we end up here program will exit, so
			 * breaks are not needed
			 */
			switch (op) {
			case 1:
				die("Error recieving filetype");
			case 2:
				die("Error recieving filename");
			case 3:
				die("Error recieving filemode");
			case 4:
				die("Error reading filecontent");
			default:
				die("Unknown error while reading data");
			}
		}

		data += rc;
		amt -= rc;

	} while (amt > 0);
}

void recieve_action(int sockfd, char *ft)
{
	read_data(sockfd, ft, sizeof(*ft), 1);
}

/*
 * The buffer passed into this function will ALWAYS
 * point to a size of STD_BUFF_SZ.
 */
void recieve_filename(int sockfd, char *buffer)
{
	zerobuf(buffer, STD_BUFF_SZ);
	read_data(sockfd, buffer, STD_BUFF_SZ, 2);
}

void recieve_filemode(int sockfd, struct stat *st)
{
	int32_t conv;

	read_data(sockfd, &conv, sizeof(int32_t), 3);
	st->st_mode = ntohl(conv);
}

void recieve_filecontent(int sockfd, const char *filename, struct stat *st)
{
	char buffer[STD_BUFF_SZ];
	FILE *fp;
	int readamt;
	int32_t conv;

	if (!(fp = fopen(filename, "w")))
		die("Error opening %s to write to", filename);

	do {
		zerobuf(buffer, STD_BUFF_SZ);

		read_data(sockfd, &conv, sizeof(int32_t), 4);
		readamt = ntohl(conv);

		read_data(sockfd, buffer, readamt, 4);

		fwrite(buffer, 1, readamt, fp);
	} while (readamt == STD_BUFF_SZ);

	fclose(fp);
	chmod(filename, st->st_mode);
}
