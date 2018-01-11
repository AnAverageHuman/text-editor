#include ".h"

int loop = 1;

int main( int argc, char *argv[] ){
  initscr();
  if (has_colors())
    start_color();
  cbreak(); // Set terminal in RAW mode
  noecho(); // Don't echo characters

  while(loop){
    getch();
  }

  endwin();
  return 0;
}
