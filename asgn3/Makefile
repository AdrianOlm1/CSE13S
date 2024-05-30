CC = gcc
SRC = test_password_checker.c password_checker.c
OBJ = $(SRC:.c=.o)
HEADER = password_checker.h

password_checker: $(OBJ)
	$(CC) $(OBJ) -o password_checker

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf password_checker
	rm -rf *.o

