#include <pthread.h>
#include <semaphore.h>

typedef struct {
  
} ;

typedef struct {
  char* buffer[10];
  pthread_mutex_t lock;
} buffer_t;

typedef struct {
  char* ip_addr;
  int port;
  int socket_fd;
} socket_t;

typedef struct {
  char* db_file;
  char* botname;
  FILE* socket_file;

  socket_t sock_data;
  buffer_t out;
  buffer_t in;
} magicbot_config_t;
