#include ".h"
#include "networking.h"

static void sighandler(int signo) {
  switch (signo) {
    case SIGINT:
      printf("Interrupted.\n");
      loop = 0;
  }
}

void client(const char *ip, const char *port) {
  signal(SIGINT, sighandler);
  initscr(); // Initializes curses
  if (has_colors()) {
    start_color();
  }
  cbreak(); // Set terminal in RAW mode
  noecho(); // Don't echo characters
  keypad(stdscr, TRUE); // Enable geting input of arrow keys
  nodelay(stdscr, false);

  int server_socket;
  char buffer[BUFFER_SIZE];

  char ch[2] = {0, 0};
  fd_set read_fds;

  server_socket = client_setup(ip, port);

  while(loop){

    //printf("enter data: ");
    ////the above printf does not have \n
    ////flush the buffer to immediately print
    //fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //printf("[Client] Reading from stdin");

      ch[0] = getch();
      ch[1] = 0;
      //printf("%c", ch);
      //*strchr(buffer, '\n') = 0;
      write(server_socket, ch, sizeof(ch));
      read(server_socket, ch, sizeof(ch));
      //printf("received: [%s]\n", buffer);
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      //printf("[Client] Reading from server");
      read(server_socket, buffer, sizeof(buffer));
      //printf("[SERVER BROADCAST] [%s]\n", buffer);
      //printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      //fflush(stdout);
    }//end socket select

  }//end loop

  endwin(); // Close window
}

