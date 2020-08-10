SRC_DIR := src
INC_DIR := include
OBJ 	:= obj
SRC 	:= $(wildcard $(SRC_DIR)/*.c)
OBJS 	:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRC))
EXE 	:= RBClient
CC 	:= gcc
CFLAGS 	:= -Wall -c

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS)/*.o

