#include "../include/FileReciever.h"

enum operation {
	R_ACTION = 0,
	R_FNAME,
	R_FMODE,
	R_FCONT,
	R_COUNT,
};

static void read_data(int sockfd, void *ptr, size_t amt, enum operation op)
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
			case R_ACTION:
				die("Error recieving action");
			case R_FNAME:
				die("Error recieving filename");
			case R_FMODE:
				die("Error recieving filemode");
			case R_FCONT:
				die("Error reading filecontent");
			case R_COUNT:
				die("Error recieving file count");
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
	read_data(sockfd, ft, sizeof(*ft), R_ACTION);
}

/*
 * The buffer passed into this function will ALWAYS
 * point to a size of STD_BUFF_SZ.
 */
void recieve_filename(int sockfd, char *buffer)
{
	zerobuf(buffer, STD_BUFF_SZ);
	read_data(sockfd, buffer, STD_BUFF_SZ, R_FNAME);
}

void recieve_filemode(int sockfd, struct stat *st)
{
	int32_t conv;

	read_data(sockfd, &conv, sizeof(int32_t), R_FMODE);
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

		read_data(sockfd, &conv, sizeof(int32_t), R_FCONT);

		readamt = ntohl(conv);
		read_data(sockfd, buffer, readamt, R_FCONT);

		fwrite(buffer, sizeof(char), readamt, fp);
	} while (readamt == STD_BUFF_SZ);

	fclose(fp);
	chmod(filename, st->st_mode);
}

void recieve_numoffiles(int sockfd, int *total)
{
	int32_t conv;

	read_data(sockfd, &conv, sizeof(int32_t), R_COUNT);
	*total = ntohl(conv);
}
