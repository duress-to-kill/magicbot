// User-defined header files begin here
#include "magicbot.h"

// Begin main function
int main( int argc, char** argv ) {
  struct params* modes = get_modes( argc, argv );

  establish_irc_session(modes);
  char* line;
  while (1) {
    line = read_remote(modes->socket_file);
    p
  }
  terminate_irc_session(modes->socket_fd);

  free(modes);
}

// Begin parsing operational modes from input parameters
struct params* get_modes( int argc, char** argv ) {
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
    fprintf( stdout, "No remote address specified, defaulting to iss.cat.pdx.edu\n" );
    strcpy( modes->ip_addr, "131.252.208.87" );
  }
  // Set the remote port to use, if specified.
  if( argc > 2 )
    modes->port = atoi( argv[2] );
  else
    fprintf( stdout, "No remote port specified, defaulting to 6667\n" );

  return modes;
}
