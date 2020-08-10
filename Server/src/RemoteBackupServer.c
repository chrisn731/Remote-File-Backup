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
#include <stdarg.h>
#include "../include/CleanDirs.h"
#include "../include/FileReciever.h"
#include "../../Shared/Helper.h"


#define PORT 8080
#define DIR_NAME_BUF_SIZE 128

int verbose = 0;

/* Kick off function for backing up. */
static int begin_backup(int connfd)
{

	char buffer[STD_BUFF_SZ];
	char state;
	struct stat st;

	for (;;) {
		recieve_filetype(connfd, &state);

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

		case 'E': /* Fallthrough */
		case 0:
		default:
			break;
		}
		break;
	}

	return 0;
}

static int open_sock(const char *ip, int *serverfd, int *connfd)
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
static int dont_delete_pls(const char *filename)
{
	return (strcmp(filename, ".") &&
		strcmp(filename, "..") &&
		strcmp(filename, "RemoteBackupServer"));
}

static int purge_dir(void)
{

	struct dirent *de;
	DIR *dr;

	if (!(dr = opendir(".")))
		die("Could not open current directory.");

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

	while ((arg = argv[1]) != NULL) {
		if (*arg != '-')
			break;
		for (;;) {
			switch (*++arg) {
			case 'b':
				continue;
			case 'v':
				verbose = 1;
				continue;
			case 0:
			default:
				break;
			}
			break;
		}
		argv++;
	}
	if (verbose)
		v_log("Opening socket.");

	open_sock("127.0.0.1", &serverfd, &connfd);

	if (verbose)
		v_log("Finished opening socket and connection established,\
			preparing directory...");

	purge_dir();

	/* At this point we have a clean directory ready for files */

	/* being_backup(connfd); */


	close(connfd);
	close(serverfd);
	return 0;
}
