#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>


static void die(const char *err, ...)
{
	va_list argp;
	va_start(argp, err);
	vfprintf(stderr, err, argp);
	va_end(argp);
	fputc('\n', stderr);
	exit(1);
}

static void printbytes(const void *arr, size_t size)
{
	const char *ptr = (const char *) arr;

	printf("Bytes: ");
	while (size-- > 0)
		printf(" %02x", *ptr++);

	printf("\n");
}


static int open_sock(int *serverfd, int *connfd)
{
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	*serverfd = socket(AF_INET, SOCK_STREAM, 0);

	if (!*serverfd)
		die("Socket creation failed.");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8080);

	if (bind(*serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		die("Bind failed.");

	if (listen(*serverfd, 3) < 0)
		die("Listen error");

	clilen = sizeof(cli_addr);
	*connfd = accept(*serverfd, (struct sockaddr *) &cli_addr, &clilen);

	if (*connfd < 0)
		die("Accept error");

	return 0;
}

static void filemaker(int connfd, int *recv)
{
	int conv;
	char buffer[512] = {0};
	FILE *fp;
	struct stat st;
	char *byte = (char *) &st.st_mode;
	read(connfd, buffer, 512);

	fp = fopen(buffer, "w");

	read(connfd, byte, sizeof(int));
	st.st_mode = ntohl(st.st_mode);
	*recv = st.st_mode;

	chmod(buffer, st.st_mode);
}

int main(int argc, char **argv)
{
	int serverfd, connfd;
	int recv = 0;

	open_sock(&serverfd, &connfd);

	filemaker(connfd, &recv);

	printf("%d\n", recv);


	close(connfd);
	close(serverfd);
	return 0;
}


