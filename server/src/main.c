/*
 * Server side of the remote backup.
 * Point of this program is to:
 * 1. Clean out the directory it is in.
 * 2. Open socket and connect to the remote backup client.
 * 3. Once connection is established, start receiving files
 *    from client and create files / dirs.
 * 4. Close connection.
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "../include/cleandirs.h"
#include "../include/filereceiver.h"
#include "../include/helper.h"
#include "../include/main.h"
#include "../include/progress.h"

/* Used to make sure we dont delete the process during directory cleaning. */
#define PROG_NAME "RBServer"

struct backup_data {
	struct stat st;
	int filesbacked;
	int totalfiles;
};

int verbose = 0;

static void print_usage(const char *user_err, ...)
{
	printf("%s\n", user_err);
	printf("Error Incorrect Usage:\n"
		"Usage: ./RBServer [FLAGS...]\n\n"
		" Flags:\n"
		"  -v, -V\tBe Verbose.\n"
		"  -b, -B\tDo a backup\n\n");
}

static void backup_dir(int connfd, char *buffer)
{
	receive_filename(connfd, buffer);
	if (verbose)
		v_log("Recieved dir: %s", buffer);
	mkdir(buffer, 0755);
	chdir(buffer);
}

static void backup_file(int connfd, char *buffer, struct backup_data *info)
{
	receive_filename(connfd, buffer);
	if (verbose)
		verbose_progressbar(buffer, ++info->filesbacked, info->totalfiles);
	receive_filemode(connfd, &info->st);
	receive_filecontent(connfd, buffer, &info->st);
	if (verbose)
		puts("Done.");
	else
		non_verbose_progressbar(++info->filesbacked, info->totalfiles);
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
	struct backup_data runtime_info = {0};

	receive_numoffiles(connfd, &runtime_info.totalfiles);
	while (1) {
		receive_action(connfd, &state);

		switch (state) {
		case 'D':
			backup_dir(connfd, buffer);
			continue;

		case 'F':
			backup_file(connfd, buffer, &runtime_info);
			continue;

		case 'R': /* Return up the directory */
			chdir("..");
			continue;

		case 'E': /* E for End of Stream */
			v_log("End of stream action recieved. Closing up...");
			break;
		case 0:
		default: /* 0 and default are the error case. */
			close(connfd);
			die("Fatal Error: Recieved bad action from client. Closing.");
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
		v_log("Cleaning Directory");

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
	if (verbose)
		v_log("Finished Cleaning. Starting backup");

	return 0;
}


int main(int argc, char **argv)
{
	int serverfd, connfd, backup;
	char *arg;

	if (argc < 2) {
		print_usage("Error: too few arguments.");;
		return 1;
	}

	while ((arg = argv[1]) != NULL) {
		if (*arg != '-') {
			print_usage("Error caught unknown flag: %s", arg);
			return 1;
		}

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
		print_usage("Backup flag not set. Nothing to do.");
		return 1;
	}

	if (verbose)
		v_log("Opening socket.");

	open_sock(&serverfd, &connfd);

	if (verbose)
		v_log("Finished opening socket and connection established,"
			" preparing directory...");

	purge_dir();

	/* At this point we have a clean directory ready for files */
	begin_backup(connfd);

	v_log("Backup Complete.");

	close(connfd);
	close(serverfd);
	return 0;
}
