

server: RemoteBackupServer.o CleanDirs.o
	gcc RemoteBackupServer.o CleanDirs.o -o RemoteBackupServer

RemoteBackupServer.o:
	gcc -c RemoteBackupServer.c

CleanDirs.o:
	gcc -c CleanDirs.c

clean:
	rm *.o
