/*
 * Create a network socket and send files through. Similar to scp.
 * This is to be used EXCLUSIVELY with the Teamspeak Server directory.
 * Any other use is undefined behavior.
 */


/*
 * TODO:
 * 1. Figure out how to efficiently pass file name
 *    through server & data file contains & cut off code #EOF#
 * FUTURE: Make server multi-thread capable
 */


/* Current plan:
 * 1. Have server clean its directory.
 * 	a. CleanDirs.c should take care of this.
 * 2. Have client read its files and send them through.
 * 	a. Still need to figure out how we will transfer the name,
 * 	   file contents, and closing each file.
 * 	b. Need to figure out how server will create a directory and
 * 	   store files at certain places.
 * 3. Close connection.
 */

#include "RemoteBackupClient.h"
#include "../Helper.h"

#define PORT 8080
#define RIP "192.168.1.35"

static int verbose = 0;
static int backup = 0;

static void print_usage(void)
{
	printf("Usage goes here\n");
}

static void send_filetype(int sockfd, char ft)
{
	int rc, left;
	char *type = &ft;

	left = sizeof(ft);
	do {
		rc = write(sockfd, type, left);

		if (rc < 0)
			die("Error while sending filetype");
		else
			left -= rc;

	} while (left > 0);
}

static int open_sock(unsigned int port, const char *ip)
{
	if (verbose)
		v_log("Opening Socket...");

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
	char BUFFER[STD_BUFF_SZ];
	FILE *fp;
	struct stat filedata;

	stat(filename, &filedata);
	fp = fopen(filename, "r");

	if (!fp)
		die("Error opening: %s", filename);


	zerobuf(BUFFER, sizeof(BUFFER));

	/* Send the filename */
	sprintf(BUFFER, "%s", filename);
	write(sockfd, BUFFER, STD_BUFF_SZ);


}

static void begin_backup(int sockfd)
{
	struct dirent *de;
	DIR *dr;

	dr = opendir(".");

	if (!dr)
		die("Could not open current directory.");

	while ((de = readdir(dr)) != NULL) {
		if (file_to_skip(de->d_name)) {
			if (de->d_type == DT_DIR) {
				send_filetype(sockfd, 'D');
				/* backup_dir(de->d_name, sockfd); */
			} else {
				send_filetype(sockfd, 'F');
				backup_file(de->d_name, sockfd);
			}
		}
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

			case 'v':
				verbose = 1;
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
