#include "../common/queue.h"
#include "../http.h"
#include "h_internal.h"

// this function is a helper to get the method type from
// the request
HttpMethodType http_get_method(char *method) {
  if (strcmp(method, "GET") == 0) {
    return HTTP_GET;
  } else if (strcmp(method, "PUT") == 0) {
    return HTTP_PUT;
  } else if (strcmp(method, "POST") == 0) {
    return HTTP_POST;
  } else {
    printf("[Server] Request type unkown\n");
    return UNKNOWN;
  }
}

// this function is used to parse
// an http request into a formated data structure
// Takes in HTTP_request and fills it with parsed info
// return 0 on success and -1 on failure
HttpRequest http_request_parse(char *request_data, u32 request_size) {
  HttpRequest request = {0};
  // we print the full request for debugging
  printf("========Request=======\n");

  // we need to mark the end of the request line
  for (u32 i = 0; i < request_size - 2; i++) {
    if (request_data[i] == '\n' && request_data[i + 1] == '\n') {
      request_data[i + 1] = '|';
    }
  }

  char *request_line = strtok(request_data, "\n");
  printf("Request Line: %s\n", request_line);
  char *request_headers = strtok(NULL, "|");
  // TODO extract the key:value pairs into a queue
  // While queue is not empty pop and create dictionary mapping
  printf("Request Headers: %s\n", request_headers);
  char *request_body = strtok(NULL, "|");
  printf("Request Body: %s\n", request_body);
  char *method = strtok(request_line, " ");
  char *uri = strtok(NULL, " ");
  char *http_version = strtok(NULL, " ");
  http_version = strtok(http_version, "/");
  http_version = strtok(NULL, "/");

  request.method = http_get_method(method);
  request.body = request_body;
  request.head = request_headers;
  request.uri = uri;
  request.version = (float)atof(http_version);

  return request;
}


void http_request_destroy(HttpRequest *request) {
  free(request->uri);
  free(request->body);
} /*frees memory allocted for request*/
