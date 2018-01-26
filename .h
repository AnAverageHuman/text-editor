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

#define min(x,y) ( { __auto_type __x = (x); __auto_type __y = (y); \
    __x < __y ? __x : __y; })

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
node *add_char_to_node(node *buf, char character, long long pos);
node *del_char_from_node(node *buf, long long pos);
node *init_buffer(node *buf, unsigned long long rows);
node *read_into_buffer(node *buf, char *filename);

void die(const char *string, char x);
int errno_handler(int en);

void client();
void server();

#endif

