CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 
TARGETS = server

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm  $(TARGETS)

