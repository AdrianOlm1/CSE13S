CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LIBS = -lcurl  # Add other libraries if needed

SRC = crawler.c pagedir.c pagescanner.c url.c bag.c hashtable.c curl.c  # Add more source files
OBJ = $(SRC:.c=.o)

TARGET = crawler

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

