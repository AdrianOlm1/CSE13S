CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
SRC = test_growing_arrays.c growing_arrays.c
OBJ = $(SRC:.c=.o)
HEADER = growing_arrays.h

growing_arrays: $(OBJ)
	$(CC) $(OBJ) -o growing_arrays

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf growing_arrays
	rm -rf *.o

