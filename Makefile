CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iincludes
TARGET = app
SRC = main.c sort.c container.c args.c io.c publication.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) *.csv

.PHONY: all clean run