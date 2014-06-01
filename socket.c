// User-defined header files begin here
#include "magicbot.h"
#include "socket.h"


char* read_remote(int socket_fd) {
  // write(connection, message, strlen(message));   // Save this til read works.

  // Prepare some values we'll need to do a robust sequential read from the remote.
  size_t buffer_length = 512;
  char* buffer =  malloc( buffer_length + 1 );
  *buffer = '\0';
  char* buffer_start;
  char* instructions = malloc( buffer_length + 1 );
  //char* raw_read_start = NULL;
  size_t read_index = 0;
  ssize_t read_size = 0;
  struct node* head = NULL;
  struct node* tail = NULL;

  *instructions = '\0';

  do { 
    // Set the input marker at the end of any data remaining in the buffer after the last parsing run.
    buffer_start = buffer + strlen(buffer);
    //raw_read_start = buffer;

    read_index = read( socket_fd, buffer_start, buffer_length - read_index );
    buffer_start[read_index] = '\0';
  
    buffer_start = buffer;
    while( strchr( buffer_start, '\n' ) != NULL ) {

      if( tail == NULL ) {
        tail = malloc( sizeof(struct node) );
        head = tail;
      } else {
        tail->next = malloc( sizeof(struct node) );
        tail = tail->next;
      }
      tail->next = NULL;
      tail->data = malloc( strspan(buffer_start, '\n' ) );
      strncpy( tail->data, buffer_start, strspan(buffer_start, '\n' ) );

      //DEBUG
      fprintf( stdout, "Read: %s\n", tail->data );
      //DEBUG
      
      buffer_start = strchrnul( buffer_start, '\n' );
    }
    
    memmove( buffer, buffer_start, strlen(buffer_start) );
  } while( strlen(buffer) > 0 );
  /*
  // DEV: Read the maximum line size of an IRC message at one time until we find one that
  // matches the bot trigger string (!card or other). Strip out garbage and return instruction.
  //while( ( read_size = read( socket_fd, buffer + read_index, buffer_length - read_index ) ) != 0 ) {
  read_index == read( socket_fd, buffer, buffer_length );
    // The complex test on the above line calls read() for an amount of data not exceeding
    // the remaining amount of space in the buffer, until either the buffer is full, or the
    // remote stops sending.
    if( read_size == -1 ) {
      fprintf( stderr, "A read error occured\n" );
      exit(1);
    }
    //read_index += read_size;
    buffer[read_index] = '\0';
  //}
  */

  // Display what we read from the remote host.
  //fprintf( stdout, "The remote server transmitted \"%s\"\n", buffer );

  free(buffer);
  //return instructions;
  return NULL;
}


int strspan(char* span_start, char span_terminator ) {
  char* span_end = strchr( span_start, span_terminator );
  return ( (uintptr_t)span_end - (uintptr_t)span_start );
}


int write_remote( int socket_fd, char* message ) {
  // DEV: write not more than maximum IRC line size to remote host. Then sleep to maintain
  // flood control before returning.
  // EDIT: Allow writing of arbitrary amounts of text, in line-size chunks, with rate-limiting.
  // Leave buffer size-limiting to client function.
}


int establish_irc_session( char* ip_addr, int port ) {
  // Create a temp to catch return codes from various functions.
  int status;

  /*
  // Define various default values if they're not passed in on the command line.
  char* desired_address = malloc(16);
  int desired_port = 6667;
  // Set the remote address to use.
  if( argc > 1 )
    strncpy(desired_address, argv[1], 15 );
  else {
    fprintf( stdout, "No remote address specified, defaulting to iss.cat.pdx.edu\n" );
    strcpy(desired_address, "131.252.208.87");
  }
  // Set the remote port to use, if specified.
  if( argc > 2 )
    desired_port = atoi(argv[2]);
  else
    fprintf( stdout, "No remote port specified, defaulting to 6667\n" );
  */

  // Request a file descriptor from the OS upon which we will build our connection.
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);   
  // Error check for a failed socket allocation.
  if( socket_fd == -1 ){
    fprintf(stderr, "Failed to open socket\n");
    fprintf( stderr, "ERRNO: %d, Error: %s\n", errno, strerror(errno) );
    exit(1);
  }

  // Prepare a struct that defines the remote server to connect to.
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port =  htons(port);
  // Translate the human-readable remote address to its binary equivalent.
  status = inet_aton(ip_addr, &server_address.sin_addr);
  // Make sure the IP address was translated correctly - a failure at this stage may mean we got
  // a bad address fron the user.
  if(status == -1){
    fprintf(stderr, "Failed to parse address: %s\nERRNO: %d, Error: %s\n", ip_addr, errno, strerror(errno) );
    exit(1);
  }

  // int bool_opt;
  // socklen_t bool_opt_len = sizeof(bool_opt);
  socklen_t bool_opt_len = 32;
  // sizeof() returns a length in bytes. connect() expects a number that represents an int, but
  // it expects it in bits. Hardcoding the above value may cause unexpected behavior with other
  // socket types, but this shouldn't be relevant for ipv4 connections (32 bits by definition).
  //
  // Call connect() to negotiate a TCP handshake with the remote host, and capture its return code.
  status = connect( socket_fd, (struct sockaddr *) &server_address, bool_opt_len );
  // Check the return code. On fail, report the error and exit.
  if( status == -1 ){
    fprintf( stderr, "Failed to connect to remote: %s\n", ip_addr );
    fprintf( stderr, "ERRNO: %d, Error: %s\n", errno, strerror(errno) );
    exit(1);
  }
  else {
    fprintf( stdout, "Successfully connected to remote: %s\n", inet_ntoa(server_address.sin_addr) );
  }

  return socket_fd;
}


void terminate_irc_session(int socket_fd) {
  // Clean up and go home.
  close(socket_fd);
  printf("Goodbye!\n");
  return;
}
