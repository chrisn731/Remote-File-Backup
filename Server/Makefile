CC 	= gcc
CFLAGS 	= -Wall -Werror
CHELPR 	= ../Shared/Helper.c

all: quick

clean:
	rm *.o

quick: src/CleanDirs.c src/FileReciever.c src/RemoteBackupServer.c src/Progress.c $(CHELPR)
	$(CC) $(CFLAGS) $^ -o RBServer

.PHONY: all server quick clean
