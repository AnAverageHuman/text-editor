#include ".h"
#include "networking.h"

int sd;
int i;

struct addrinfo *hints;
struct addrinfo *results;

/* creates, binds a server side socket and sets it to the listening state
 * returns the socket descriptor
 */
int server_setup() {

  //create the socket
  sd = socket(AF_INET, SOCK_STREAM, 0);
  errno_handler(errno);

  //setup structs for getaddrinfo
  hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4 address
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  hints->ai_flags = AI_PASSIVE;  //Use all valid addresses
  getaddrinfo(arguments.ip, arguments.port, hints, &results);

  //bind the socket to address and port
  i = bind(sd, results->ai_addr, results->ai_addrlen);
  if (errno_handler(errno) == EACCES) {
    fprintf(stderr, "Permission denied while trying to bind to port %s.\n",
        arguments.port);
  }

  //set socket to listen state
  i = listen(sd, 10);
  errno_handler(errno);

  struct sockaddr_in sin;
  socklen_t sinlen = sizeof(sin);
  getsockname(sd, (struct sockaddr *) &sin, &sinlen);
  printf("This is text-editord, running on port %d\n",
      ntohs(sin.sin_port));

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  return sd;
}

/* sd should refer to a socket in the listening state
 * runs the accept call; returns the socket descriptor for the new socket
 * connected to the client.
 */
int server_connect(int sd) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);

  client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
  errno_handler(errno);

  return client_socket;
}

/* to_server is a string representing the server address
 * create and connect a socket to a server socket that is in the listening state
 * returns the file descriptor for the socket
 */
int client_setup() {
  //create the socket
  sd = socket(AF_INET, SOCK_STREAM, 0);
  errno_handler(errno);
  return sd;
}

int client_connect(int sd) {
  //run getaddrinfo
  // hints->ai_flags not needed because client specifies desired address.
  hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  getaddrinfo(arguments.ip, arguments.port, hints, &results);

  //connect to the server
  //connect will bind the socket for us
  i = connect(sd, results->ai_addr, results->ai_addrlen);
  errno_handler(errno);

  free(hints);
  freeaddrinfo(results);
  return i;
}

