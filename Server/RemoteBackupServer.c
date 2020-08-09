/*
 * Server side of the remote backup.
 * Point of this program is to:
 * 1. Clean out the backup directory.
 * 2. Open socket and connect to the remote backup client.
 * 3. Once connection is established, start receiving files
 *    from client and create files / dirs.
 * 4. Close connection.
 */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include "CleanDirs.h"
#include "FileMaker.h"
#include "../Helper.h"


#define PORT 8080
#define NAMEBUFFER 256

static int verbose = 0;

static int begin_backup(int connfd)
{

	char currdir[NAMEBUFFER] = {'.', '/'};
	char state;
	int rc, left;

	for (;;) {
		left = sizeof(state);

		do {
			rc = read(connfd, &state, left);

			if (rc < 0)
				die("Read error in backup phase.");
			else
				left -=	rc;

		} while (left > 0);

		switch (state) {
		case 'D':
			/* Do some shit for dir making */
			continue;

		case 'F':
			/* Do some shit for file making */
			continue;
		case 'E':
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

	dr = opendir(".");

	if (!dr)
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

	open_sock("127.0.0.1", &serverfd, &connfd);

	printf("Finished opening socket and connection established,\
		preparing directory...\n");

	purge_dir();

	/* At this point we have a clean directory ready for files */




	close(connfd);
	close(serverfd);
	return 0;
}
