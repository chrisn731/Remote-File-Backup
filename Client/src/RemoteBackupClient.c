/*
 * Create a network socket and send files through. Similar to scp.
 * One benefit of this over scp is that it will recursively send all
 * types of files through a socket.
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include "../include/Helper.h"
#include "../include/FileBackup.h"
#include "../include/ProgressBar.h"

static unsigned int totalfilecount = 0;
static unsigned int totalfilesbacked = 0;
int verbose = 0;

static void print_usage(const char *user_err)
{
	printf("%s\n", user_err);
	printf("Usage: ./RBClient [FLAGS...]\n\n"
		" Flags:\n"
		"  -v, -V\tBe Verbose.\n"
		"  -b, -B\tDo a backup.\n"
		"  --ip\t\tSet ip of remote host.\n\n");
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
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if (verbose)
		v_log("Attempting to connect to server");

	/* Keep trying to connect until connection is made */
	while (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		sleep(2);

	if (verbose)
		v_log("Connection established.");

	return sock;
}

/*
 * Function to skip certain filenames while parsing directory.
 * Returns 0 if it is a file to skip, != 0 otherwise.
 */
static inline int file_to_backup(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, "..") &&
		strcmp(filename, "ts3server.pid"));
}

/* Backs up a singular file */
static void backup_file(const char *filename, const int sockfd)
{
	struct stat filedata;

	stat(filename, &filedata);

	send_action(sockfd, 'F');
	send_filename(sockfd, filename);
	send_filemode(sockfd, &filedata);
	if (verbose)
		send_filecontent_verbosely(sockfd, filename, &filedata);
	else
		send_filecontent(sockfd, filename, ++totalfilesbacked, totalfilecount);
}

/*
 * Backs up a whole directory and sends actions to server based
 * on the types of files in the directory. Types of actions are:
 * F: Incoming file.
 * D: Incoming Directory, create and recurse into it.
 * R: Return to previous directory.
 * E: End of data stream.
 */
static void backup_dir(int sockfd, const char *path)
{
	struct dirent *de;
	DIR *dr;

	if (!(dr = opendir(path)))
		die("Could not open directory: %s", path);
	if (chdir(path))
		die("Error: Failed to change into directory: %s", path);

	if (verbose)
		v_log("Attempting to backup directory: %s", path);

	while ((de = readdir(dr)) != NULL) {
		if (file_to_backup(de->d_name)) {
			if (de->d_type == DT_DIR) {
				send_action(sockfd, 'D');
				send_filename(sockfd, de->d_name);
				backup_dir(sockfd, de->d_name);
				send_action(sockfd, 'R');
				chdir("..");
			} else {
				backup_file(de->d_name, sockfd);
			}
		}
	}
	closedir(dr);
}

/* Kick off function for backing up a given directory */
static void begin_backup(int sockfd, const char *path)
{
	/* Begin backup with the directory that the program is in */
	send_filecount(sockfd, totalfilecount);
	backup_dir(sockfd, path);

	/* Send 'E'nd to the server to tell it to stop. */
	send_action(sockfd, 'E');
}

int main(int argc, char **argv)
{
	char *arg, *IP = NULL;
	int sockfd, backup;

	if (argc < 2) {
		print_usage("Error: Too few arguemnts");
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
			case 'V':
			case 'v':
				verbose = 1;
				continue;
			case '-':
				if (!strcmp("-ip", arg)) {
					IP = argv[2];
					argv++;
				}
				break;
			case 0:
			default:
				break;
			}
			break;
		}
		argv++;
	}
	if (!backup) {
		print_usage("Error: Backup flag not set. Nothing to do.");
		return 1;
	}
	if (!IP) {
		print_usage("Error: Remote host IP not set.");
		return 1;
	}
	sockfd = open_sock(PORT, IP);
	totalfilecount = num_of_files(".");

	printf("Beginning Backup...\n");
	begin_backup(sockfd, ".");
	printf("Backup complete.\n");

	close(sockfd);
	return 0;
}
