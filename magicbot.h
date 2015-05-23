#ifndef MAGICBOT_H
#define MAGICBOT_H

// System header files begin here
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

// Command parsing functionality
params* get_modes( int argc, char** argv );

// IRC I/O functionality
char* read_remote(FILE* stream);
int write_remote(int socket_fd, char* message, size_t message_length );
void terminate_irc_session(int socket_fd);
int strspan(char* span_start, char span_terminator );
char** parse(char* raw);
void process_queries(params* modes, char** tokv);

// Json fetching functionality

// Database manipulation functionality

#endif
