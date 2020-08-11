CC 	= gcc
CFLAGS 	= -Wall -Werror

server: RemoteBackupServer.o CleanDirs.o FileReciever.o
	gcc RemoteBackupServer.o CleanDirs.o FileReciever.o -o RemoteBackupServer

RemoteBackupServer.o: RemoteBackupServer.c
	$(CC) $(CFLAGS) RemoteBackupServer.c

CleanDirs.o: CleanDirs.c
	$(CC) $(CFLAGS) CleanDirs.c

FileReciever.o: FileReciever.c
	$(CC) $(CFLAGS) FileReciever.c

clean:
	rm *.o

quick: src/CleanDirs.c src/FileReciever.c src/RemoteBackupServer.c ../Shared/Helper.c
	$(CC) $(CFLAGS) $^ -o RBServer

.PHONY: all server
