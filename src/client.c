#include ".h"
#include "networking.h"

#define RETRY_TIMER 10

int server_socket;
node *thebuffer;
char *statusbarc;
char *commandlinec;

WINDOW *statusbarw;
WINDOW *commandlinew;
int col;
int row;
unsigned long long currentc;
unsigned long long currentr;
node *currentnode;

static void sighandler(int signo) {
  switch (signo) {
    case SIGINT:
      printf("Interrupted.\n");
      loop = 0;
  }
}

void destroy_screen() {
  delwin(statusbarw);
  delwin(commandlinew);
  free(statusbarc);
  free(commandlinec);
  endwin(); // close window
}

void init_screen() {
  initscr();
  atexit(destroy_screen);

  if (has_colors()) {
    start_color();
  }
  cbreak(); // Set terminal in RAW mode
  nonl(); // enable getting input of enter
  noecho(); // Don't echo characters
  keypad(stdscr, TRUE); // Enable geting input of arrow keys
  halfdelay(2); // 1/5 of a second

  getmaxyx(stdscr, col, row);
  statusbarw = newwin(1, row - 2, 0, 0);
  commandlinew = newwin(1, row - 1, 0, 0);

  statusbarc = malloc(sizeof(char) * col);
  commandlinec = malloc(sizeof(char) * col);
}

void redraw_line(node *buf, int r, char m) {
  mvwaddnstr(stdscr, r, 0, buf->contents, min(col, buf->length));
  move(r, buf->length);
  clrtoeol();
  if (m) {
    move(currentr, currentc);
  }
}

void redraw_all() {
  node *cur = thebuffer;
  getmaxyx(stdscr, row, col);
  int tmpr = 0;
  while (cur && tmpr < row) { // main window
    redraw_line(cur, tmpr, 0);
    cur = cur->next;
    tmpr++;
  }

  waddnstr(statusbarw, statusbarc, col); // status bar
  waddnstr(commandlinew, commandlinec, col); // command line
  move(currentr, currentc);
  refresh();
}

void client() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, SIG_IGN);

  if (! isatty(1)) {
    fprintf(stderr, "Not a TTY; bailing out.\n");
    exit(1);
  }

  currentnode = thebuffer = init_buffer(0, 1);
  if (arguments.filename) {
    read_into_buffer(thebuffer, arguments.filename);
  }

  char buffer[BUFFER_SIZE];
  int ch[2] = {0, 0};
  fd_set read_fds;

  init_screen();
  redraw_all();

  while (loop) {
    switch(ch[0] = getch()) {
      case ERR:
        break;
      case KEY_ENTER:
        currentnode = add_node(0, currentnode, currentnode->next, 0, 0, 0);
        currentr++;
        currentc = 0;
        break;
      case KEY_UP:
        if (currentnode->prev) {
          currentr--;
          currentnode = currentnode->prev;
          currentc = min(currentc, currentnode->length - 1);
        }
        break;
      case KEY_DOWN:
        if (currentnode->next) {
          currentr++;
          currentnode = currentnode->next;
          currentc = min(currentc, currentnode->length - 1);
        }
        break;
      case KEY_LEFT:
        if (currentc > 0) {
          currentc--;
        }
        break;
      case KEY_RIGHT:
        if (currentc < currentnode->length - 1) {
          currentc++;
        }
        break;
      case KEY_BACKSPACE:
        if (currentnode->contents[currentc - 1]) {
          del_char_from_node(currentnode, currentc);
          redraw_line(currentnode, currentr, 1);
          currentc--;
        }
        break;
      default: // probably a "normal" character
        add_char_to_node(currentnode, ch[0], currentc);
        redraw_line(currentnode, currentr, 1);
        currentc++;
        break;
    }
    move(currentr, currentc);
  }

/*
  fprintf(stderr, "Connecting to text-editord running on %s:%s\n",
      arguments.ip,
      arguments.port);

  server_socket = client_setup();
  while (client_connect(server_socket) == -1) {
    fprintf(stderr, "Failed to connect, retrying in %d seconds", RETRY_TIMER);
    sleep(RETRY_TIMER);
  }

  while(loop){

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      ch[0] = getch();
      ch[1] = 0;
      send(server_socket, ch, sizeof(ch), 0);
      recv(server_socket, ch, sizeof(ch), 0);
      //printf("received: [%s]\n", buffer);
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      //printf("[Client] Reading from server");
      recv(server_socket, buffer, sizeof(buffer), 0);
      //printf("[SERVER BROADCAST] [%s]\n", buffer);
      //printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      //fflush(stdout);
    }//end socket select

  }//end loop

*/

  destroy_screen();
}

