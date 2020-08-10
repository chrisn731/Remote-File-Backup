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

#include "../include/RemoteBackupClient.h"
#include "../../Shared/Helper.h"
#include "../include/FileBackup.h"

#define PORT 8080
#define RIP "192.168.1.35"

static int backup = 0;
int verbose = 0;

static void print_usage(void)
{
	printf("Usage goes here\n");
}


static int open_sock(unsigned int port, const char *ip)
{
	int sock;
	struct sockaddr_in serv_addr;

	if (verbose)
		v_log("Opening Socket...");

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
static int can_delete(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, "..") &&
		strcmp(filename, "ts3server.pid"));
}

static void backup_file(const char *filename, const int sockfd)
{
	struct stat filedata;

	if (verbose)
		v_log("Attempting to backup %s", filename);

	stat(filename, &filedata);

	send_filetype(sockfd, 'F');

	send_filename(sockfd, filename, STD_BUFF_SZ);

	send_filemode(sockfd, &filedata);

	send_filecontent(sockfd, filename);
}

static void begin_backup(int sockfd)
{
	struct dirent *de;
	DIR *dr;

	dr = opendir(".");

	if (!dr)
		die("Could not open current directory.");

	while ((de = readdir(dr)) != NULL) {
		if (can_delete(de->d_name)) {
			if (de->d_type == DT_DIR) {
				send_filetype(sockfd, 'D');
				send_filename(sockfd, de->d_name, sizeof(de->d_name));
				/* Switch dir */
			} else {
				backup_file(de->d_name, sockfd);
			}
		}
	}

	/*
	 * Once here, we need to send ~EOS~ so the server knows
	 * we dont have anything else to send and it should stop listening.
	 */

	closedir(dr);
}

int main(int argc, char **argv)
{
	char *arg;
	int sockfd;

	verbose = 0;

	if (argc < 2) {
		/* Usage will go here */
		print_usage();
		return 1;
	}

	while ((arg = argv[1]) != NULL) {
		if (*arg != '-')
			die("Error caught unknown flag: %s", arg);

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

	/* Initiate the backup here */

	printf("Backup complete.\n");
	close(sockfd);
	return 0;

}
