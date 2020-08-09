SRC_DIR = src
HDR_DIR = include
SRC 	= $(wildcard $(SRC_DIR)/*.c)

all: RemoteBackupClient.o
	gcc -Wall RemoteBackupClient.o -o RemoteBackupClient

RemoteBackupClient.o: RemoteBackupClient.c
	gcc -c RemoteBackupClient.c

clean:
	rm *.o

.PHONY: all
