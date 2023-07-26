.PHONY : all clean

CFLAGS := -Wall -lrt
CC := gcc

client:
	$(CC) -o client client.c $(CFLAGS)

server:
		$(CC) -o server server.c $(CFLAGS)
clean:
	rm -rf  client server