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
  char *ip;        // string
  char *port;      // port
  char *filename;
};

extern char loop;
extern struct arguments arguments;

typedef struct node {
  unsigned long long length;    // actual length
  unsigned long long allocated; // amount allocated
  char *contents;
  struct node *prev;
  struct node *next;
} node;

node *add_node(node *buf, node *prev, node *next, char *contents,
    long long length, long long allocd);
node *init_buffer(node *buf, unsigned long long rows);

void die(const char *string, char x);
int errno_handler(int en);

void client();
void server();

#endif

