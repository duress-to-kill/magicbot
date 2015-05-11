// User-defined header files begin here
#include "magicbot.h"

// Begin main function
int main(int argc, char** argv) {
  params* modes = get_modes( argc, argv );

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
params* get_modes(int argc, char** argv) {
  params* modes = malloc(sizeof(params));

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
  // Syntax spec: requests take form of one of:
  // !card(name)
  // !card(name:field1,field2,field3)
  // other search terms may be implemented later, with form !foo()
  // the most likely implementation to follow will be !grep(token), which should return
  //   a list of cardnames that match a substring match for "token"
  char* start;
  int span;
  const int bufferlen = 300;
  const int tokenlen = 100;
  const int tokencount = 10;

  // Check to ensure that all necessary syntax tokens are found in the raw input
  if((start = strchr(raw, '!')) == NULL         // check for !
      || (start = strchr(start, '(')) == NULL   // check for subsequent (
      || (span = strspan(start, ')')) == -1)  // check for ) following initial (
    return NULL;
  start++;     // If syntax checking passed, move the parsing bounds in by 1, so
  span -= 2;   //   start and end position are both inside the enclosing parens

  char** retv = NULL;
  char tokv[tokencount][tokenlen + 1];
  char buffer[bufferlen];
  char delim[2] = ":";
  int i = 0;

  strncpy(buffer, start, span);
  /*
  if(strcmp(buffer, "card") == 0) {
    // ***bookmark***
    start = start[span + 1];
    if((span = strspan(start, ')') == -1)
      return NULL;
    strncpy(buffer, start, span - 1);
    if((
  } // else if(strcmp( other keywords ) == 0)
  */
  start = strtok(buffer, delim);
  strncpy(tokv[i], start, tokenlen);
  i++;
  delim = ",";
  while(i < tokencount && (start = strtok(NULL, delim)) != NULL) {
    strncpy(tokv[i], start, tokenlen);
    i++;
  }
  retv = malloc();
  // ********************* BOOKMARK ************************* //

  return retv;
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
