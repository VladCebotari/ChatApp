CC = gcc
CFLAGS = -Wall -g $(shell pkg-config --cflags gtk4) -pthread
LIBS   = $(shell pkg-config --libs gtk4) -pthread


all: server client

server: server.o
	$(CC) server.o -o server

server.o: server.c
	$(CC) -c server.c -o server.o


client: client.o UI.o
	$(CC) client.o UI.o -o client $(LIBS)

client.o: client.c UI.h
	$(CC) -c client.c $(CFLAGS) -o client.o

UI.o: UI.c UI.h
	$(CC) -c UI.c $(CFLAGS) -o UI.o

clean:
	rm -f *.o client server

