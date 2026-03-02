#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../types.h"

#define PORT 8080
#define MAX_BACKLOG 10
#define MAX_ENTITY_SIZE

#define STATUS_OK 200
#define STATUS_NOT_FOUND "404"

// enumeration for the different
// http methods which is held in
// the request struct
typedef enum { HTTP_GET, HTTP_POST, HTTP_PUT } HttpMethodType;

// structure to hold a
// servers information:
// port, socket, and address
// binded to
struct HttpServer{
  u16 port;
  u32 socket;
  struct sockaddr_in server_addr;
};

// structure needed
// for the http request
// information body, header, url, and type
struct HttpRequest{
  HttpMethodType method;
  char *uri;
  char *body;
  char *head;
};

struct HttpResponse{
  char *status_line;
  /*(general header | response header | entity header)*/
  u32 entity_length;
  // CRLF
  char *entity_body;
}; 

