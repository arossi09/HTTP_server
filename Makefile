CC = gcc
CFLAGS = -g -Wall -Werror -Wpedantic -O2 -std=c99 
TARGETS = http_server

SRC= http_server.c \
		 http/h_request.c \
		 http/h_response.c\
		 http/h_server.c 

OBJ= $(SRC:.c=.o)

all: $(TARGETS)

$(TARGETS): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f *.o
	rm -f http/*.o
	rm  $(TARGETS)

