#include "../http.h"
#include "h_internal.h"

// this function is used to parse
// an http request into a formated data structure
// Takes in HTTP_request and fills it with parsed info
// return 0 on success and -1 on failure
HttpRequest *http_request_parse(char request_data[MAX_REQUEST_SIZE],
                                u32 request_size) {
  // we print the full request for debugging
  for (u32 i = 0; i < request_size; i++) {
    printf("%c", request_data[i]);
  }
  // we need to extract request type
  char *next = strtok(request_data, " ");
  if (!next) {
    printf("[Server] Could not read request type\n");
    return NULL;
  }
  // handle different requests
  if (strcmp(next, "GET") == 0) {
    printf("[Server] get request!\n");
    return http_request_get_parse(next);
  } else if (strcmp(next, "PUT") == 0) {
    return http_request_put_parse(next);
  } else if (strcmp(next, "POST") == 0) {
    return http_request_post_parse(next);
  } else {
    printf("[Server] Request type unkown\n");
    return NULL;
  }
  return 0;
}

HttpRequest *http_request_get_parse(char *request_byte_stream) {
  HttpRequest *http_request = (HttpRequest *)malloc(sizeof(HttpRequest));
  if (!http_request) {
    printf("[Server] failed to allocate memory for request\n");
    return NULL;
  }
  request_byte_stream = strtok(NULL, " ");
  if (!request_byte_stream) {
    printf("[Server] could not read file from get request \n");
    return NULL;
  }
  // TODO if streln of token exceed some limit send back 414
  http_request->uri = (char *)malloc(strlen(request_byte_stream));
  if (!*http_request->uri) {
    printf("[Server] Failed to allocate memory for get uri\n");
    return NULL;
  }
  strcpy(http_request->uri, request_byte_stream);
  http_request->method = HTTP_GET;
  return http_request;
}

HttpRequest *http_request_post_parse(char *request_byte_stream) { return NULL; }
HttpRequest *http_request_put_parse(char *request_byte_stream) { return NULL; }

void http_request_destroy(HttpRequest *request) {
  return;
} /*frees memory allocted for request*/
