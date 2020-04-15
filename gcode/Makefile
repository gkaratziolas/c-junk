PROJ_NAME=gcode_reader
CC=gcc
CFLAGS=-std=c99
OBJ=$(PROJ_NAME)

SRC = *.c

CFLAGS += -I.

$(PROJ_NAME): $(SRC)
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)
clean:
	rm $(OBJ)
