CC 	:= gcc
CFLAGS += -std=c99 -Wpedantic -pedantic-errors
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Waggregate-return
CFLAGS += -Wbad-function-cast
CFLAGS += -Wcast-align
CFLAGS += -Wcast-qual
CFLAGS += -Wdeclaration-after-statement
CFLAGS += -Wfloat-equal
CFLAGS += -Wformat=2
CFLAGS += -Wlogical-op
CFLAGS += -Wmissing-declarations
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wnested-externs
CFLAGS += -Wpointer-arith
CFLAGS += -Wredundant-decls
CFLAGS += -Wsequence-point
CFLAGS += -Wshadow
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wswitch
CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wunused-but-set-parameter
CFLAGS += -Wwrite-strings
CFLAGS += -D_DEFAULT_SOURCE
CFLAGS += -O2
HELPERC := ../Shared/Helper.c
EXE 	:= RBClient

.PHONY: all clean

all: quick

clean:
	rm $(OBJ)/*.o

quick: src/FileBackup.c src/RemoteBackupClient.c src/Progress.c $(HELPERC)
	$(CC) $(CFLAGS) $^ -o $(EXE)
