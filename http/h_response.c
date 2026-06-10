#include "../http.h"
#include "h_internal.h"

char *http_response_get_content_type(const char *path) {
  const char *ext = strrchr(path, '.');

  if (!ext)
    return "application/octet-stream";

  if (strcmp(ext, ".html") == 0)
    return "text/html; charset=utf-8";
  if (strcmp(ext, ".css") == 0)
    return "text/css; charset=utf-8";

  return "application/octet-stream";
}

// this function is used to create a response from a file
// request
HttpResponse http_response_file_create(const char *root, HttpRequest request) {
  HttpResponse http_response = {0};

  char path[MAX_PATH];

  // add index.html to endpath if uri
  // requested is a directory
  int n = snprintf(path, sizeof(path), "%s%s", root, request.uri);

  int len = strlen(request.uri);
  if (request.uri[len - 1] == '/') {
    n = snprintf(path, sizeof(path), "%sindex.html", path);
  }

  if (n < 0 || n >= (int)sizeof(path)) {
    http_response.status = HTTP_STATUS_BAD_REQUEST;
    return http_response;
  }

  // TODO sanitize the request uri to not allow '..'
  printf("trying to open %s\n", path);
  int fd = open(path, O_RDONLY);

  // we need to send 404 if uri not found
  if (fd < 0) {
    printf("[Server] get request uri not found: %s\n", path);
    http_response.status = HTTP_STATUS_NOT_FOUND;
    return http_response;
  }

  // we need to gather the file size requested
  lseek(fd, 0, SEEK_END);
  u32 file_size = lseek(fd, 0, SEEK_CUR);
  lseek(fd, 0, SEEK_SET);
  /*
  if (file_size > MAX_FILE_SIZE) {
    printf("[Server] file size exceeds max file size\n");
    http_response.status = HTTP_STATUS_MAX_FILE_SIZE;
    return http_response;
  }
  */

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
    return http_response;
  }
  http_response.status = HTTP_STATUS_OK;
  http_response.content_type = http_response_get_content_type(path);
  close(fd);
  return http_response;
}

HttpResponse *http_response_put_create(HttpRequest *request) { return NULL; }
HttpResponse *http_response_post_create(HttpRequest *request) { return NULL; }

// head function for orchestrating response creation based on the
// request method
HttpResponse http_response_create(HttpRequest request) {
  switch (request.method) {
  case HTTP_GET:
    return http_response_file_create("/Users/ant/Projects/webserver/webpage",
                                     request);
  default: {
    HttpResponse response = {0};
    response.status = HTTP_STATUS_METHOD_NOT_ALLOWED;
    return response;
  }
  }
}

// this function is used to send back a formated HttpResponse
// to the client
int http_response_send(HttpResponse response, i32 client_socket) {
  const char *status_text = NULL;
  switch (response.status) {
  case HTTP_STATUS_OK:
    status_text = "200 OK";
    break;
  case HTTP_STATUS_NOT_FOUND:
    status_text = "404 Not Found";
    break;
  case HTTP_STATUS_MAX_FILE_SIZE:
    status_text = "500 Internal Server Error";
    break;
  default:
    break;
  }

  printf("=====Response=====\n");

  // format the response correctly and send
  char header[MAX_HEADER_SIZE];
  int header_len = snprintf(header, sizeof(header),
                            "HTTP/1.1 %s \r\n"
                            "%s\r\n\r\n",
                            response.content_type, status_text);
  if (header_len < 0 || header_len >= MAX_HEADER_SIZE) {
    printf("[Server] exceeded max header size\n");
    return 0;
  }
  for (int i = 0; i < header_len; i++) {
    printf("%c", header[i]);
  }

  send(client_socket, header, header_len, 0);

  if (response.entity_body && response.entity_length > 0) {
    send(client_socket, response.entity_body, response.entity_length, 0);
    for (int i = 0; i < response.entity_length; i++) {
      printf("%c", response.entity_body[i]);
    }
  }
  return 1;
}

extern void http_response_destroy(HttpResponse *response) {
  if (!response)
    return;
  free(response->entity_body);
  response->entity_body = NULL;
  response->entity_length = 0;
}
