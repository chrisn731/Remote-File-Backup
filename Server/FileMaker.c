#include "FileMaker.h"
#include "../Helper.h"

static int c_fd = 0;

static void fill_file_contents(FILE *fp)
{

	fclose(fp);
}

static void get_mode(struct stat *st)
{
	int rc;
	int left = sizeof(st->st_mode);
	char *byte = (char *) &(st->st_mode);

	do {
		rc = read(c_fd, byte, left);

		if (rc < 0) {
			/* die error */
		} else {
			byte += rc;
			left -= rc;
		}
	} while (left > 0);

	st->st_mode = ntohl(st->st_mode);
}

int make_file(int connfd)
{
	FILE *fp;
	char filename[512];
	int left, rc;
	struct stat st;
	c_fd = connfd;

	left = sizeof(filename);
	do {
		rc = read(connfd, filename, left);

		if (rc < 0)
			die("Error reading filename in make_file");
		else
			left -= rc;
	} while (left > 0);

	fp = fopen(filename, "w");

	get_mode(&st);

	if (chmod(filename, st.st_mode))
		fprintf(stderr, "%s\n", "chmod failed");

	/* Transfer file contents to here */




	return 0;

}
