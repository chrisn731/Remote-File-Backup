/*
 * Server side of the remote backup.
 * Point of this program is to:
 * 1. Clean out the backup directory.
 * 2. Open socket and connect to the remote backup client.
 * 3. Once connection is established, start receiving files
 *    from client and create files / dirs.
 * 4. Close connection.
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "../include/CleanDirs.h"
#include "../include/FileReciever.h"
#include "../../Shared/Helper.h"

/* Used to make sure we dont delete the process during directory cleaning. */
#define PROG_NAME "RBServer"

static int backup = 0;
int verbose = 0;

static void print_usage(void)
{
	printf("Error Incorrect Usage:\n"
		"./RBServer [-v/V] [-b/B] [--ip {IP}]\n"
		"-v/V\tTurn on verbose\n"
		"-b/B\tDo a backup\n"
		"--ip\tSet ip of host.\n");
}

/*
 * Kick off function for backing up. Recieves an action from a client,
 * and performs some action based on it.
 * F: Incoming file.
 * D: Incoming directory, create dir and chdir into it.
 * R: Return to parent directory.
 * E: End of data stream.
 */
static int begin_backup(int connfd)
{

	char buffer[STD_BUFF_SZ];
	char state;
	struct stat st;

	for (;;) {
		recieve_action(connfd, &state);

		switch (state) {
		case 'D':
			recieve_filename(connfd, buffer);
			v_log("Recieved dir: %s", buffer);
			mkdir(buffer, 0755);
			chdir(buffer);
			/* Do some shit for dir making */
			continue;

		case 'F':
			/* Do some shit for file making */
			recieve_filename(connfd, buffer);
			v_log("Recieved file: %s", buffer);
			recieve_filemode(connfd, &st);
			recieve_filecontent(connfd, buffer, &st);
			continue;

		case 'R':
			chdir("..");
			continue;

		case 'E':
			v_log("End of stream action recieved. Closing up...");
			break;
		case 0:
		default:
			break;
		}
		break;
	}
	return 0;
}

static int open_sock(int *serverfd, int *connfd)
{
	int addrlen;
	struct sockaddr_in servaddr, cli;

	*serverfd = socket(AF_INET, SOCK_STREAM, 0);

	if (!*serverfd)
		die("Socket creation failed.");

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if (bind(*serverfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		die("Bind failed.");

	if (listen(*serverfd, 3) < 0)
		die("listen error");

	addrlen = sizeof(cli);
	*connfd = accept(*serverfd, (struct sockaddr *) &cli, (socklen_t *) &addrlen);

	if (*connfd < 0)
		die("Accept error");

	return 0;
}

/*
 * Dont delete certain files while purging.
 * Returns 0 if file name matches a file to not delete.
 * Otherwize returns != 0.
 */
static inline int dont_delete_pls(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, "..") &&
		strcmp(filename, PROG_NAME));
}

static int purge_dir(void)
{
	struct dirent *de;
	DIR *dr;

	if (!(dr = opendir(".")))
		die("Could not open current directory.");

	if (verbose)
		v_log("Purging Directory");

	while ((de = readdir(dr)) != NULL) {
		if (dont_delete_pls(de->d_name)) {
			if (de->d_type == DT_DIR) {
				CleanDirectory(de->d_name);
			} else {
				if (remove(de->d_name))
					die("Error deleting %s", de->d_name);
			}
		}
	}

	closedir(dr);
	return 0;
}


int main(int argc, char **argv)
{
	int serverfd, connfd;
	char *arg;

	if (argc < 2) {
		print_usage();
		return 1;
	}

	while ((arg = argv[1]) != NULL) {
		if (*arg != '-')
			die("Error caugh unknown flag: %s", arg);

		for (;;) {
			switch (*++arg) {
			case 'B':
			case 'b':
				backup = 1;
				continue;
			case 'V':
			case 'v':
				verbose = 1;
				continue;
			case '-':
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
		print_usage();
		return 1;
	}

	if (verbose)
		v_log("Opening socket.");

	open_sock(&serverfd, &connfd);

	if (verbose)
		v_log("Finished opening socket and connection established,"
			"preparing directory...");

	purge_dir();

	if (verbose)
		v_log("Finished Cleaning. Starting backup");

	/* At this point we have a clean directory ready for files */

	begin_backup(connfd);

	if (verbose)
		v_log("Finished backup. Cleaning up...");

	close(connfd);
	close(serverfd);

	if (verbose)
		v_log("Done.");

	return 0;
}
