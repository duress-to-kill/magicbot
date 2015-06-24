// User-defined header files begin here
#include "magicbot.h"

// Begin main function

// Begin parsing operational modes from input parameters
params* get_modes(int argc, char** argv) {
  params* modes = malloc(sizeof(params));

  // Define various default values if they're not passed in on the command line.
  modes->ip_addr = malloc(16);
  modes->port = 6667;
  modes->botname = malloc(9);
  strncpy(modes->botname, "oraclev2", 8);

  // Set the remote address to use.
  if( argc > 1 )
    strncpy(modes->ip_addr, argv[1], 15);
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
  if(raw == NULL) {
    printf("Warning: Read null string from remote. Ignoring input.\n");
    return NULL;
  }

  char* start;
  char* arg_start;
  int span;
  const int bufferlen = 500;
  const int tokenlen = 100;
  const int tokencount = 12;

  // Catch server PINGs ane respond.
  if((start = strstr(raw, ":PING ")) != NULL) {
    start++;

    char** retv = malloc(sizeof(char*) * 3);
    retv[0] = strdup(start);
    retv[0][4] = 'O';  // Make the PONG reply from the initial PING
    retv[1] = retv[0] + 5;
    return retv;
  }

  // Check to ensure that all necessary syntax tokens are found in the raw input
  if((start = strstr(raw, "PRIVMSG")) == NULL         // check for !
      || (start = strchr(start, '!')) == NULL         // check for !
      || (arg_start = strchr(start, '(')) == NULL   // check for subsequent (
      || (span = strspan(arg_start, ')')) == -1)  // check for ) following initial (
    return NULL;
  arg_start++;  // Shift the start marker past the opening paren. The closing paren will be used to stop parsing, and will be stripped out later.

  char** retv = malloc(sizeof(char*) * (tokencount + 1));
  memset(retv, 0, sizeof(char*) * (tokencount + 1));
  char tokbuffer[tokenlen + 1];
  char buffer[bufferlen];
  char delim[2] = ":";

  // Capture command name
  retv[0] = malloc(sizeof(char) * strspan(start, '('));
  strncpy(retv[0], start + 1, strspan(start, '('));
  *strchr(retv[0], '(') = '\0';

  // If the parameter list has a length greater than 0, capture the first word as the parameter
  if(span > 1) {
    int i = 1;
    strncpy(buffer, arg_start, span);
    arg_start = strtok(buffer, delim);
    strncpy(tokbuffer, arg_start, tokenlen - 1);
    retv[i] = malloc(sizeof(char) * strlen(tokbuffer) + 1);
    strcpy(retv[i], tokbuffer);
    if((start = strchr(retv[i], ')')) != NULL) {  // If the parameter string ends after the first token, stop parsing here.
      *start = '\0';
    } else {  // Otherwise, start capturing subparameters until the closing paren is found, or the limit of 10 is reached.
      i++;
      strcpy(delim,",");
      while(i < tokencount && (arg_start = strtok(NULL, delim)) != NULL) {
        strncpy(tokbuffer, arg_start, tokenlen);
        tokbuffer[tokenlen] = '\0';
        retv[i] = malloc(strlen(tokbuffer) + 1);
        strcpy(retv[i], tokbuffer);
        if((start = strchr(retv[i], ')')) != NULL) {
          *start = '\0';
        }
        i++;
      }
    }
  }

  return retv;
}

void process_queries(params* modes, char** tokv) {
  char* output = malloc(512);
  int len = 0;
  int i;

  // BEGIN DEBUG
  len = snprintf(output, 510, "PRIVMSG #mtg_test :Read command \'%s\', with the following parameter and subparameters:", tokv[0]);
  for(i = 1; len < 510 && tokv[i] != NULL; i++) {
    len += snprintf(&output[len], 510 - len, " %s", tokv[i]);
  }
  len += sprintf(&output[len], "\n");
  write_remote(modes->socket_fd, output, len);
  free(output);
  return;
  // END DEBUG
}

