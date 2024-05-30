CC = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(subst .c,.o,$(SOURCES))

.PHONY: clean

all : histogram 

histogram : histogram.o
	$(CC) -o histogram $(OBJECTS)

%.o : %.c
	$(CC) -c $<

clean:
	rm -f histogram $(OBJECTS)
