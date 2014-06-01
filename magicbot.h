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

// Command parsing functionality
struct params* get_modes( int argc, char** argv );
struct params {
    char* ip_addr;
    int port;
};

// IRC I/O functionality
char* read_remote(int socket_fd);
int write_remote(int socket_fd, char* message);
int establish_irc_session( char* ip_addr, int port );
void terminate_irc_session(int socket_fd);

// Json fetching functionality

// Database manipulation functionality

#endif
