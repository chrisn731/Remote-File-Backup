CC 	:= gcc
CFLAGS 	:= -Wall -Werror
HELPERC := ../Shared/Helper.c
EXE 	:= RBClient

.PHONY: all clean

all: quick

clean:
	rm $(OBJ)/*.o

quick: src/FileBackup.c src/RemoteBackupClient.c src/Progress.c $(HELPERC)
	$(CC) $(CFLAGS) $^ -o $(EXE)
