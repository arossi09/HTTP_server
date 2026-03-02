#include "types.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

//opaque defintions later internalized within h_internal
typedef struct HttpResponse HttpResponse;
typedef struct HttpRequest HttpRequest;
typedef struct HttpServer HttpServer;
#define MAX_REQUEST_SIZE 1024

/* ------------ HTTP server controls ------------ */
/*starts listening to connections on the addressspecificed*/
extern HttpServer *http_server_create(u16 port);
/*starts listening to connections on the addressspecificed*/
extern int http_server_start(HttpServer *server);
/*handles requests and responding to connection*/
extern void http_server_connection_handle(u32 client_socket);
/*starts listening to connections on the addressspecificed*/
extern void http_server_destroy(HttpServer *server);

/* ------------ HTTP request parsing ------------ */
extern HttpRequest *http_request_post_parse(char *request_byte_stream);
extern HttpRequest *http_request_put_parse(char *request_byte_stream);
extern HttpRequest *http_request_get_parse(char *request_byte_stream);
extern HttpRequest *http_request_parse(char request_data[MAX_REQUEST_SIZE],
                                       u32 request_size);
extern void http_request_destroy(
    HttpRequest *request); /*frees memory allocted for request*/

/* ------------ HTTP response creation ------------ */
extern HttpResponse *http_response_get_create(
    HttpRequest *request); /*creates a response based on the request*/
extern HttpResponse *http_response_put_create(HttpRequest *request);
extern HttpResponse *http_response_post_create(HttpRequest *request);
extern int
http_response_send(HttpResponse *response,
                   i32 client_socket); /*sends the response to the client*/
extern void http_response_destroy(
    HttpResponse *response); /*frees memory allocated for response*/
