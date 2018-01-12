#include ".h"
#include "networking.h"

int loop = 1;

void client( int argc, char *argv[] ){

  int server_socket;
  char buffer[BUFFER_SIZE];

  char ch[2] = {0, 0};
  fd_set read_fds;

  if (argc >= 3)
    server_socket = client_setup( argv[2]);
  else
    server_socket = client_setup( TEST_IP );

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
      printf("[Client] Reading from server");
      read(server_socket, buffer, sizeof(buffer));
      //printf("[SERVER BROADCAST] [%s]\n", buffer);
      //printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      //fflush(stdout);
    }//end socket select

  }//end loop
  
}

void print_help(){
  printf("Usage:\n./text-editor --server\n./text-editor --client\n");
}

int main( int argc, char *argv[] ){

  if(argc >= 2){
    printf("%s\n", argv[1]);
    if(!strcmp( argv[1], "--server" )){
      server();
    } else if(!strcmp( argv[1], "--client")){
      initscr(); // Initializes curses
      if (has_colors())
        start_color();
      cbreak(); // Set terminal in RAW mode
      noecho(); // Don't echo characters
      keypad(stdscr, TRUE); // Enable geting input of arrow keys
      nodelay(stdscr, false);
      client( argc, argv );
      endwin(); // Close window
    } else {
      print_help();
    }
  } else {
    print_help();
  }


  //while(loop){
  //}

  //return 0;
}
