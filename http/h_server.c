#include "../http.h"
#include "h_internal.h"
#include <pthread.h>
#include <semaphore.h>

// TODO limit connections to 64 via semaphor
// TODO add timeout to each socket
// TODO add nginx reverse proxy for TLS, etc.

struct thread_arg {
  i32 socket;
  sem_t *sem;
};

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

// this function handles parsing requests from clients
// and responding based off whether  the request was a GET, POST, or PUT
void *http_server_connection_handle(void *socket /*void *arg*/) {
  printf("[Server] servicing requests from client\n");
  printf("[Server] Request received\n");
  // struct thread_arg *args = arg;

  // sem_wait(args->sem);

  i32 client_socket = (i32)(intptr_t)socket;

  char request_buff[MAX_REQUEST_SIZE];
  u32 bytes_read = read(client_socket, request_buff, MAX_REQUEST_SIZE);
  // we need to parse request and if invalid then said a error back
  HttpRequest http_request = http_request_parse(request_buff, bytes_read);
  HttpResponse http_response = {0};
  // we switch on request and handle seperatly
  http_response = http_response_create(http_request);
  // we respond to the client with the formated response
  http_response_send(http_response, client_socket);
  http_response_destroy(&http_response);
  close(client_socket);
  pthread_exit(NULL);

  // sem_post(args->sem);
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

  /*
  struct thread_arg targs;
  sem_t sem = 64;

  targs.sem = &sem;
  */

  while ((client_sockfd =
              accept(http_server->socket, (struct sockaddr *)&client_address,
                     (socklen_t *)&client_size)) > 0) {
    printf("[Server] Client Connected!\n");

    // targs.socket = client_sockfd;
    pthread_t thread;

    if (pthread_create(&thread, NULL, http_server_connection_handle,
                       (void *)(intptr_t)client_sockfd) != 0) {
      close(client_sockfd);
      continue;
    }

    pthread_detach(thread);
  }
  close(http_server->socket);
  return 0;
}

void http_server_destroy(HttpServer *server) {
  if (server) {
    free(server);
  }
}
