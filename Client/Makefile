CC 	:= gcc
CFLAGS 	:= -Wall
SRC_DIR := ./src
OBJ_DIR := ./obj
SRCS 	:= $(wildcard $(SRC_DIR)/*.c)
HELPERC := ../Shared/Helper.c
OBJS 	:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
EXE 	:= RBClient

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS) $(OBJ_DIR)/Helper.o
	$(CC) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c | ($OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Helper.o: $(HELPERC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ)/*.o
