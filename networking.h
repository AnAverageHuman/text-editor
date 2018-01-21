#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 256

void error_check(int i, char *s);
int server_setup();
int server_connect(int sd);
int client_setup();

#endif

