#include "http.h"

//TODO implement response creation for get and response send

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./webserver [PORT_NUMBER]\n");
    return 1;
  }
  u16 port = atoi(argv[1]);
  HttpServer *server = http_server_create(port);
	if(!server){
		perror("Failed to create http server");
		return -1;
	}
  if(http_server_start(server) < 0){
		perror("Failed to start http server!");
		return -1;
	}
  http_server_destroy(server);
  return 0;
}
