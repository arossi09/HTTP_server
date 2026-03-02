#include "h_internal.h"
#include "../http.h"

HttpResponse *http_response_get_create(HttpRequest *request) {
  HttpResponse *http_response = (HttpResponse *)malloc(sizeof(HttpResponse));
  if (!http_response) {
    printf("[Server] couldnt allocate memory for response\n");
    return NULL;
  }
  // TODO sanitize the request uri to not allow '..'
  int fd = open(request->uri, O_RDONLY, S_IRUSR);
  // we need to send 404 if uri not found
  if (fd < 0) {
    printf("[Server] get request uri not found: %s\n", request->uri);
    http_response->status_line = (char *)malloc(sizeof(STATUS_NOT_FOUND));
		if(!http_response->status_line){
			printf("[Server] couldn't allocate for get failed status line\n");
			return NULL;
		}
    strcpy(http_response->status_line, STATUS_NOT_FOUND);
    return NULL;
  }
	//we need to gather the file size requested
  lseek(fd, 0, SEEK_END);
  u32 file_size = lseek(fd, 0, SEEK_CUR);
  lseek(fd, 0, SEEK_SET);
  // grab the resource and load file
  http_response->entity_length = file_size;
  // TODO check if file_size is bigger than max file_size if not then allocate
  // space in array and read in memory
	return http_response;
}
HttpResponse *http_response_put_create(HttpRequest *request) { return NULL; }
HttpResponse *http_response_post_create(HttpRequest *request) { return NULL; }

// this function is used to send back a formated HttpResponse
// to the client
int http_response_send(
    HttpResponse *response,
    i32 client_socket) { // TODO send the response to clinet socket
  return 1;
}

extern void http_response_destroy(
    HttpResponse *response){
	return;
}
