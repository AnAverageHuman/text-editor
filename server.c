#include ".h"
#include "networking.h"

static void sighandler(int signo) {
  switch (signo) {
    case SIGINT:
      printf("Interrupted.\n");
      loop = 0;
  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (recv(client_socket, buffer, sizeof(buffer), 0)) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void server() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, SIG_IGN);
  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();

  while(loop) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket = server_connect(listen_socket);

      if (fork()) {
        subserver_count++;
        close(client_socket);
      } else {
        subserver(client_socket);
      }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] subserver count: %d\n", subserver_count);
      if(!strcmp(buffer, "exit\n"))
        loop = 0;
    }//end stdin select
  }
}

