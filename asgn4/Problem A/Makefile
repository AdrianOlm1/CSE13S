CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
SRC = test_ll_cycle.c ll_cycle.c
OBJ = $(SRC:.c=.o)
HEADER = ll_cycle.h

check_cycle: $(OBJ)
	$(CC) $(OBJ) -o check_cycle

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf check_cycle
	rm -rf *.o

