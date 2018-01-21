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

extern char loop;

void die(const char *string, char x);

void client(const char *ip, const char *port);
void server(const char *ip, const char *port, const char *filename);

#endif

