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
  for (u32 i = 0; i < request_size; i++) {
    printf("%c", request_data[i]);
  }

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

  // we need to gather all header key value pairs
  queue_t headers;
  queue_create(&headers);
  char *key_pair_entry = strtok(request_headers, "\n");
  while (key_pair_entry) {
    queue_enqueue(&headers, key_pair_entry);
    key_pair_entry = strtok(NULL, "\n");
  }

  char *header = (char *)queue_dequeue(&headers);
  while (!queue_is_empty(&headers)) {
		//add 
		//char *key = strtok(header, ":");
		//char *value = strtok(NULL, "|");
		//TODO add this pair to a dictionary
    header = queue_dequeue(&headers);
    printf("Size %lu, Header: %s\n", queue_size(&headers), header);
  }
  return request;
}

HttpRequest *http_request_get_parse(char *request_byte_stream) {
  HttpRequest *http_request = (HttpRequest *)malloc(sizeof(HttpRequest));
  if (!http_request) {
    printf("[Server] failed to allocate memory for request\n");
    return NULL;
  }
  request_byte_stream = strtok(NULL, " ");
  printf("Test: %s\n", request_byte_stream);
  if (request_byte_stream) {
    printf("[Server] could not read file from get request: %s\n",
           request_byte_stream);
    return NULL;
  }
  // TODO read header
  http_request->head = NULL;
  // TODO if streln of token exceed some limit send back 414
  http_request->uri = (char *)malloc(strlen(request_byte_stream));
  if (!*http_request->uri) {
    printf("[Server] Failed to allocate memory for get uri\n");
    free(http_request);
    return NULL;
  }
  strcpy(http_request->uri, request_byte_stream);
  http_request->method = HTTP_GET;
  return http_request;
}

HttpRequest *http_request_post_parse(char *request_byte_stream) { return NULL; }
HttpRequest *http_request_put_parse(char *request_byte_stream) { return NULL; }

void http_request_destroy(HttpRequest *request) {
  free(request->uri);
  free(request->body);
} /*frees memory allocted for request*/
