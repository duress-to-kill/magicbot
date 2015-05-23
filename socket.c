// User-defined header files begin here
#include "magicbot.h"


/* this function is borked
char* read_remote(int socket_fd) {
  // write(connection, message, strlen(message));   // Save this til read works.

  // Prepare some values we'll need to do a robust sequential read from the remote.
  size_t buffer_length = 512;
  char* buffer = malloc(buffer_length + 1);
  // *buffer = '\0';
  memset(buffer, '\0', buffer_length + 1);
  char* buffer_start;
  //char* instructions = malloc( buffer_length + 1 );
  size_t read_index = 0;
  //ssize_t read_size = 0;
  struct node* head = NULL;
  struct node* tail = NULL;

  // *instructions = '\0';

  do { 
    // Set the input marker at the end of any data remaining in the buffer after the last parsing run.
    buffer_start = buffer + strlen(buffer);

    read_index = read(socket_fd, buffer_start, buffer_length - read_index);
    buffer_start[read_index] = '\0';

    buffer_start = buffer;
    while(strchr(buffer_start, '\n') != NULL) {
      printf("DEBUG 1");
      fflush(stdin);

      if(tail == NULL) {
        tail = malloc(sizeof(struct node));
        head = tail;
      } else {
        tail->next = malloc(sizeof(struct node));
        tail = tail->next;
      }
      tail->next = NULL;
      tail->data = malloc(strspan(buffer_start, '\n'));
      strncpy(tail->data, buffer_start, strspan(buffer_start, '\n'));

      //DEBUG
      fprintf(stdout, "Read: %s\n", tail->data);
      free(tail->data);
      free(tail);
      tail = NULL;
      //free(instructions);
      //DEBUG

      buffer_start = (char*)(strchrnul(buffer_start, '\n') + 1);
    }

    memmove(buffer, buffer_start, strlen(buffer_start) + 1);
  } while(strlen(buffer) > 0);

  free(buffer);
  //return instructions;
  return NULL;
}
*/

char* read_remote(FILE* stream) {
  char buffer[513];
  memset(buffer, '\0', 513);
  //recv(socket_fd, buffer, 512, 0);
  if(fgets(buffer, 512, stream) != NULL)
  {
    printf("Read: %s", buffer);
    char* payload = NULL;
    if(buffer != NULL) {
      payload = malloc(strlen(buffer) + 1);
      strcpy(payload, buffer);
    }
    return payload;
  }
  return NULL;
}


int write_remote(int socket_fd, char* message, size_t message_length) {
  // DEV: write not more than maximum IRC line size to remote host. Then sleep to maintain
  // flood control before returning.
  // EDIT: Allow writing of arbitrary amounts of text, in line-size chunks, with rate-limiting.
  // Leave buffer size-limiting to client function.
  //while( strlen(message) > 0 ) {
    printf("Write: %s\n", message);
    write(socket_fd, message, message_length);
    write(socket_fd, "\n", 1);
  //}

  return 0;
}


// A management function called form main() that handles building a network connection, identifying to
// nickserv, and all other connection-related functionality.
void establish_irc_session(params* modes) {
  // Open a network socket and reord the coresponding file descriptor.
  modes->socket_fd = build_connection( modes->ip_addr, modes->port );
  modes->socket_file = fdopen(modes->socket_fd, "w+");

  char message[511];
  sprintf(message, "USER %s %s %s :%s\n", modes->botname, modes->botname, modes->botname, modes->botname);
  write_remote(modes->socket_fd, message, strlen(message));
  sprintf(message, "NICK %s\n", modes->botname);
  write_remote(modes->socket_fd, message, strlen(message));
  sprintf(message, "JOIN #mtg_test\n");
  write_remote(modes->socket_fd, message, strlen(message));

}


int build_connection( char* ip_addr, int port ) {
  // Create a temp to catch return codes from various functions.
  int status;

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
  if(status == -1){
    fprintf(stderr, "Failed to connect to remote: %s\n", ip_addr);
    fprintf(stderr, "ERRNO: %d, Error: %s\n", errno, strerror(errno));
    exit(1);
  }
  else {
    fprintf(stdout, "Successfully connected to remote: %s\n", inet_ntoa(server_address.sin_addr));
  }


//* TESTING
  //write_remote(socket_fd, "USER oraclebot oraclebot oraclebot :oraclebot\n", 46);
  //write_remote(socket_fd, "NICK oraclebotv2\n", 17);
//*/
  //printf("\nSubmitted server data...\n");
  //write_remote(socket_fd, "JOIN #mtg\n", 9);
  //printf("\nSubmitted channel data...\n");
  return socket_fd;
}


void terminate_irc_session(int socket_fd) {
  // Clean up and go home.
  close(socket_fd);
  printf("Goodbye!\n");
  return;
}
