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

// IRC I/O functionality
void establish_irc_session(int argc, char** argv);
char* read_remote();
int write_remote(char* message);
void terminate_irc_session();

// Json fetching functionality

// Command parsing functionality

// Database manipulation functionality

