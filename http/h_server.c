#include "../http.h"
#include "h_internal.h"

// we need this function in order to start
// the server passed
HttpServer *http_server_create(u16 port) {
  HttpServer *http_server = (HttpServer *)malloc(sizeof(HttpServer));
	http_server->port = port;
  if (!http_server) {
    printf("[Server] failed to allocate memory for http server\n");
    return NULL;
  }
  // we need to set the addres and type for our socket
  http_server->server_addr.sin_family = AF_INET;
  http_server->server_addr.sin_port = htons(port);
  http_server->server_addr.sin_addr.s_addr = INADDR_ANY;

  // we need to create socket under ipv4
  http_server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (http_server->socket < 0) {
    perror("sockfd");
    return NULL;
  }

  // bind the socket to address
  i16 rc =
      bind(http_server->socket, (struct sockaddr *)&http_server->server_addr,
           sizeof(http_server->server_addr));
  if (rc < 0) {
    perror("bind");
    return NULL;
  }
  printf("[Server] binded to address on port %d\n", http_server->port);
  return http_server;
}

i32 http_server_start(HttpServer *http_server) {
  printf("[Server] listening for connections...\n");
  // while listening accept connections
  if (listen(http_server->socket, MAX_BACKLOG) < 0) {
    perror("listen");
    return -1;
  }

  // handle connections
  i32 client_sockfd;
  struct sockaddr_in client_address;
  u32 client_size = sizeof(client_address);
  while ((client_sockfd =
              accept(http_server->socket, (struct sockaddr *)&client_address,
                     (socklen_t *)&client_size)) > 0) {
    printf("[Server] Client Connected!\n");
    printf("%d\n", client_sockfd);
    http_server_connection_handle(client_sockfd);
  }
  close(http_server->socket);
  return 0;
}

// this function handles parsing requests from clients
// and responding based off whether  the request was a GET, POST, or PUT
void http_server_connection_handle(u32 client_socket) {
  printf("[Server] servicing requests from client: %d\n", client_socket);
  printf("[Server] Request received\n");
  char request_buff[MAX_REQUEST_SIZE];
  u32 bytes_read = read(client_socket, request_buff, MAX_REQUEST_SIZE);
  // we need to parse request and if invalid then said a error back
  HttpRequest *http_request = http_request_parse(request_buff, bytes_read);
  HttpResponse *http_response = NULL;
  if (!http_request) {
    char *buf = "HTTP/1.0 400 Bad Request\r\n\r\n";
    u32 bytes_sent = send(client_socket, buf, strlen(buf) * sizeof(char), 0);

    if (bytes_sent != strlen(buf)) {
      printf("[Server] byte mismatch \n\tsent: %d \n\tsize of message: %lu",
             bytes_sent, strlen(buf));
    }
    return;
  }
  // http_response_create()
  // we switch on request and handle seperatly
  switch (http_request->method) {
  case HTTP_GET:
    http_response = http_response_get_create(http_request);
    break;
  case HTTP_PUT:
    // send 501
    break;
  case HTTP_POST:
    // send 501
    break;
  default:
    printf("[Server] unrecognized request\n");
    char *buf = "HTTP/1.0 400 Bad Request\r\n\r\n";
    u32 bytes_sent = send(client_socket, buf, strlen(buf) * sizeof(char), 0);

    if (bytes_sent != strlen(buf)) {
      printf("[Server] byte mismatch \n\tsent: %d \n\tsize of message: %lu",
             bytes_sent, strlen(buf));
    }
  }
  // we respond to the client with the formated response
  http_response_send(http_response, client_socket);

  /*
char *buf = "HTTP/1.0 200 OK\r\n\r\n";
u32 bytes_sent = send(client_socket, buf, strlen(buf) * sizeof(char), 0);

if (bytes_sent != strlen(buf)) {
printf("[Server] byte mismatch \n\tsent: %d \n\tsize of message: %lu",
     bytes_sent, strlen(buf));
}*/
  http_response_destroy(http_response);
  http_request_destroy(http_request);
  close(client_socket);
}

void http_server_destroy(HttpServer *server) {
  // TODO free server memory
}
