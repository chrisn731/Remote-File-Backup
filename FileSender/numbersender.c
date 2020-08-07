#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>

#define PORT 8080
#define RIP "192.168.1.35"


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

int main(void)
{
	int sockfd;
	int x = 321;

	sockfd = open_sock(PORT, RIP);

	/*
	printf("Size of struct stat: %d\n", sizeof(struct stat));
	printf("Attempting to send %04o\n", st.st_mode);
	*/

	int conv = htonl(x);
	write(sockfd, &conv, sizeof(int));
	close(sockfd);

	return 0;
}

