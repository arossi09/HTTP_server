#include "../http.h"
#include "h_internal.h"

HttpResponse http_response_get_create(HttpRequest request) {
  HttpResponse http_response = {0};
	char path[30000] = {0};
	strcpy(path, "/Users/ant/Projects/webserver");
	strcat(path, request.uri);
  printf("trying to open %s\n", path);
  // TODO sanitize the request uri to not allow '..'
  int fd = open(path, O_RDONLY, S_IRUSR);
  // we need to send 404 if uri not found
  if (fd < 0) {
    // TODO need to have this handle return
    printf("[Server] get request uri not found: %s\n", path);
    http_response.status = HTTP_STATUS_NOT_FOUND;
    return http_response;
  }
  // we need to gather the file size requested
  lseek(fd, 0, SEEK_END);
  u32 file_size = lseek(fd, 0, SEEK_CUR);
  lseek(fd, 0, SEEK_SET);
  // grab the resource and load file
  http_response.entity_length = file_size;
  // we need to allocate memory for the entity body
  http_response.entity_body = malloc(http_response.entity_length);
  if (!http_response.entity_body) {
    printf("[Server] couldn't allocate for get entity body\n");
    return http_response;
  }
  i32 bytes_read =
      read(fd, http_response.entity_body, http_response.entity_length);
  if (bytes_read < 0 || bytes_read != http_response.entity_length) {
    printf("[Server] failed to read entity body for get response\n");
    free(http_response.entity_body);
    return http_response;
  }
  http_response.status = HTTP_STATUS_OK;
  // TODO check if file_size is bigger than max file_size if not then
  // allocate space in array and read in memory
  return http_response;
}
HttpResponse *http_response_put_create(HttpRequest *request) { return NULL; }
HttpResponse *http_response_post_create(HttpRequest *request) { return NULL; }

// this function is used to send back a formated HttpResponse
// to the client
int http_response_send(
    HttpResponse response,
    i32 client_socket) { // TODO send the response to clinet socket

  switch (response.status) {
  case HTTP_STATUS_OK:
    send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n",
         STATUS_OK_LINE_SIZE, 0);
    break;
  case HTTP_STATUS_NOT_FOUND:
    break;
  default:
    break;
  }
  send(client_socket, response.entity_body, response.entity_length, 0);
  return 1;
}

extern void http_response_destroy(HttpResponse *response) {
  free(response->entity_body);
}
