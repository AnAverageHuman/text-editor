#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <argp.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

struct arguments {
  char servermode; // boolean
  char *ip;        // string
  char *port;      // port
  char *filename;
};

extern char loop;
extern struct arguments arguments;

void die(const char *string, char x);
int errno_handler(int en);

void client();
void server();

#endif

