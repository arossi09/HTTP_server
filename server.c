#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

#define PORT 8080
#define MAX_BACKLOG 10
#define MAX_REQUEST_SIZE 1024

// structure to hold a
// servers information:
// port, socket, and address
// binded to
typedef struct {
  u16 port;
  u32 socket;
  struct sockaddr_in server_addr;
} HTTP_Server;

// enumeration for the different
// http methods which is held in
// the request struct
typedef enum { HTTP_GET, HTTP_POST, HTTP_PUT } HTTP_Method;

// structure needed
// for the http request
// information body, header, url, and type
typedef struct {
  HTTP_Method method;
  char *url;
  char *body;
  char *head;
} HTTP_Request;



//this function is needed for parsing get
int parse_get(){
	return 0;
}

int parse_put(){
	return 0;
}

int parse_post(){
	return 0;
}

// this function is used to parse
// an http request into a formated data structure
// Takes in HTTP_request and fills it with parsed info
// return 0 on success and -1 on failure
int parse_request(HTTP_Request *request, char request_data[MAX_REQUEST_SIZE],
                  u32 request_size) {
  // we print the full request for debugging
  for (u32 i = 0; i < request_size; i++) {
    printf("%c", request_data[i]);
  }
  // we need to extract request type
  char *next = strtok(request_data, " ");
  if (!next) {
    printf("[Server] Could not read request type\n");
    return -1;
  }
	//handle different requests
  if (strcmp(next, "GET") == 0) {
    printf("[Server] get request!\n");
		return parse_get();
  } else if (strcmp(next, "PUT") == 0) {
		return parse_put();
  } else if (strcmp(next, "POST") == 0) {
		return parse_post();
  } else {
    printf("[Server] Request type unkown\n");
    return -1;
  }

  return 0;
}

// we need this function to handle new connections
// to requests
void handle_connection(u32 client_socket) {
  printf("[Server] servicing requests from client: %d\n", client_socket);
  printf("[Server] Request received\n");
  char request_buff[MAX_REQUEST_SIZE];
  HTTP_Request request;
  u32 bytes_read = read(client_socket, request_buff, MAX_REQUEST_SIZE);
  // we need to parse request and if invalid then said a error back
  if (parse_request(&request, request_buff, bytes_read) < 0) {
    char *buf = "HTTP/1.0 400 Bad Request\r\n\r\n";
    u32 bytes_sent = send(client_socket, buf, strlen(buf) * sizeof(char), 0);

    if (bytes_sent != strlen(buf)) {
      printf("[Server] byte mismatch \n\tsent: %d \n\tsize of message: %lu",
             bytes_sent, strlen(buf));
    }
    return;
  }

  char *buf = "HTTP/1.0 200 OK\r\n\r\n";
  u32 bytes_sent = send(client_socket, buf, strlen(buf) * sizeof(char), 0);

  if (bytes_sent != strlen(buf)) {
    printf("[Server] byte mismatch \n\tsent: %d \n\tsize of message: %lu",
           bytes_sent, strlen(buf));
  }
  close(client_socket);
}

// we need this function in order to start
// the server passed
int start_server(HTTP_Server *server) {
  // we need to set the addres and type for our socket
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_port = htons(server->port);
  server->server_addr.sin_addr.s_addr = INADDR_ANY;

  // we need to create socket under ipv4
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket < 0) {
    perror("sockfd");
    return -1;
  }

  // bind the socket to address
  i16 rc = bind(server->socket, (struct sockaddr *)&server->server_addr,
                sizeof(server->server_addr));
  if (rc < 0) {
    perror("bind");
    return -1;
  }
  printf("[Server] binded to address\n");
  printf("[Server] listening for connections...\n");
  // while listening accept connections
  if (listen(server->socket, MAX_BACKLOG) < 0) {
    perror("listen");
    return -1;
  }

  // handle connections
  u32 client_sockfd;
  struct sockaddr_in client_address;
  u32 client_size = sizeof(client_address);
  while ((client_sockfd =
              accept(server->socket, (struct sockaddr *)&client_address,
                     (socklen_t *)&client_size)) > 0) {
    printf("[Server] Client Connected!\n");
    printf("%d\n", client_sockfd);
    handle_connection(client_sockfd);
  }
  close(server->socket);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./webserver [PORT_NUMBER]\n");
    return 1;
  }
  u16 port = atoi(argv[1]);
  HTTP_Server server;
  server.port = port;
  if (start_server(&server) < 0) {
    perror("start_server");
    exit(1);
  }
  close(server.socket);
  return 0;
}
