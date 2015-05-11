typedef struct {
  char* ip_addr;
  int port;
  int socket_fd;
  char* botname;
  FILE* socket_file;
} params;

typedef struct {
  char* data;
  struct node* next;
} node;

int write_remote( int socket_fd, char* message, size_t message_length );
void establish_irc_session( params* modes );
int build_connection( char* ip_addr, int port );
void terminate_irc_session(int socket_fd);
