
CC=gcc
CFLAGS=-I .
INC_FILES=hello.h
.PHONY: all clear

%.o: %.c $(INC_FILES)
	gcc -c -o $@ $< $(CFLAGS)
all: main.o hello.o
	$(CC) -o main main.o hello.o $(CFLAGS)
clear:
	rm -rf main
clear1:
	rm -rf *.o