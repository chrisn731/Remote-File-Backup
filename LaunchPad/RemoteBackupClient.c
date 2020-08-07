/*
 * Create a network socket and send files through. Similar to scp.
 * This is to be used EXCLUSIVELY with the Teamspeak Server directory.
 * Any other use is undefined behavior.
 */


/*
 * TODO:
 * 1. Set up function to open socket.
 * 2. Figure out how to efficiently pass file name
 *    through server & data file contains & cut off code #EOF#
 * 3. Get other laptop operational for demo
 * 4. Make server multi-thread capable
 */


/* Current plan:
 * 1. Establish connection to Remote Backup server.
 * 	a. open_sock() should take care of this.
 * 2. Have server clean its directory.
 * 	a. CleanDirs.c should take care of this.
 * 3. Have client read its files and send them through.
 * 	a. Still need to figure out how we will transfer the name,
 * 	   file contents, and closing each file.
 * 	b. Need to figure out how server will create a directory and
 * 	   store files at certain places.
 * 4. Close connection.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "RemoteBackupClient.h"

#define PORT 8080
#define RIP "192.168.1.35"
#define FTBS 512 /* File transfer buffer size */

static int verbose = 0;
static int backup = 0;

static void die(const char *err, ...)
{
	va_list argp;
	va_start(argp, err);
	vfprintf(stderr, err, argp);
	va_end(argp);
	fputc('\n', stderr);
	exit(1);
}

static void print_usage(void)
{
	printf("Usage goes here\n");
}

static int strcmp(const char *p1, const char *p2)
{
	char c1, c2;

	do {
		c1 = *p1++;
		c2 = *p2++;

		if (c1 == '\0')
			return c2 - c1;

	} while (c1 == c2);

	return c2 - c1;
}

static void zerobuf(char *buf, size_t bufsize)
{
	int i;

	if (bufsize <= 0)
		die("Sent <=0 to zerobuf function.");

	for (i = 0; i < bufsize; i++)
		buf[i] = '\0';
}

static int open_sock(unsigned int port, const char *ip)
{
	int sock;
	struct sockaddr_in serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		die("Socket Creation error.");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr(RIP);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		die("Connection Failed.");

	return sock;
}

/*
 * Function to skip certain filenames while parsing directory.
 * Returns 0 if it is a file to skip, != 0 otherwise.
 */
static int file_to_skip(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, "..") &&
		strcmp(filename, "ts3server.pid"));
}

static void backup_file(const char *filename, const int sockfd)
{
	char BUFFER[FTBS];
	FILE *fp;


	fp = fopen(filename, "r");

	if (!fp)
		die("Error opening: %s", filename);


	zerobuf(BUFFER, FTBS);

	/* Send the filename */
	sprintf(BUFFER, "%s", filename);
	write(sockfd, BUFFER, FTBS);

	fread(BUFFER, 1, FTBS - 1, fp);
	write(sockfd, BUFFER, FTBS);

}

static void begin_backup(int sockfd)
{
	struct dirent *de;
	DIR *dr;
	char MODEBUF[32];

	dr = opendir(".");

	if (!dr)
		die("Could not open current directory.");

	while ((de = readdir(dr)) != NULL) {
		if (!file_to_skip(de->d_name))
			continue;

		/*
		 * At this point we have the names of all files and dirs in
		 * main directory. We still need to recurse through the
		 * subdirs and catch all of those files.
		 */
		if (de->d_type == DT_DIR)
			continue;

		backup_file(de->d_name, sockfd);
	}

	closedir(dr);
}

int main(int argc, char **argv)
{
	char *arg;
	int sockfd;

	if (argc < 2) {
		/* Usage will go here */
		print_usage();
		return 1;
	}

	while ((arg = argv[1]) != NULL) {

		if (*arg != '-')
			die("Error caught unknown flag.");

		for (;;) {
			switch (*++arg) {
			case 'b':
			case 'B':
				/* backup shit */
				backup = 1;
				continue;

			case 0:
				break;
			default:
				break;
			}
			break;
		}
		argv++;
	}

	sockfd = open_sock(PORT, RIP);

	printf("Backup complete.\n");
	close(sockfd);
	return 0;

}
