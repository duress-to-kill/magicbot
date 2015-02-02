// User-defined header files begin here
#include "magicbot.h"

// Begin main function
int main(int argc, char** argv) {
  struct params* modes = get_modes( argc, argv );

  establish_irc_session(modes);
  char* line;
  char** tokv;
  while (1) {
    line = read_remote(modes->socket_file);
    tokv = parse(line);
    free(line);
    if (tokv != NULL) {
      process_queries(tokv);
    //free tokv
    }
  }
  terminate_irc_session(modes->socket_fd);

  free(modes);
}

// Begin parsing operational modes from input parameters
struct params* get_modes(int argc, char** argv) {
  struct params* modes = malloc(sizeof(struct params));

  // Define various default values if they're not passed in on the command line.
  modes->ip_addr = malloc(16);
  modes->port = 6667;
  modes->botname = malloc(9);
  strncpy( modes->botname, "oraclev2", 8 );

  // Set the remote address to use.
  if( argc > 1 )
    strncpy( modes->ip_addr, argv[1], 15 );
  else {
    fprintf(stdout, "No remote address specified, defaulting to iss.cat.pdx.edu\n");
    strcpy(modes->ip_addr, "131.252.208.87");
  }
  // Set the remote port to use, if specified.
  if(argc > 2)
    modes->port = atoi(argv[2]);
  else
    fprintf(stdout, "No remote port specified, defaulting to 6667\n");

  return modes;
}

char** parse(char* raw) {
  char* start;
  int span;
  if((start = strchr(raw, '!')) == NULL || ((span = strspan(start, '(')) == -1))
    return NULL;

  char bufferv[10][100];
  char buffer[100];
  int i = 0;

  strncpy(buffer, start, span - 1);
  if(strcmp(buffer, "card") == 0) {
    ***bookmark***
  } // else if(strcmp( other keywords ) == 0)
}

void process_queries(char** tokv) {
  
}

int strspan(char* span_start, char span_terminator ) {
  char* span_end = strchr( span_start, span_terminator );
  if(span_end == NULL)
    return -1;
  int span = (uintptr_t)span_end - (uintptr_t)span_start;
  return span;
}
