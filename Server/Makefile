CC 	= gcc
CFLAGS 	= -c -Wall

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

.PHONY: all server
