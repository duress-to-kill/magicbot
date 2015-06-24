#include <pthread.h>

static void init_config(magicbot_config_t* config) {
  config->db_file = (char*)chk_malloc(malloc(strlen("AllCards-x.json") + 1));
  strncpy(config->db_file, "AllCards-x.json", 16);
  config->botname = (char*)chk_malloc(malloc(strlen("oraclev2") + 1));
  strncpy(config->botname, "oraclev2", 9);
  config->sock_data.ip_addr = (char*)chk_malloc(malloc(16)); // strlen("123.123.123.123") + 1
  config->sock_data.ip_addr[0] = '\0';
  int i;
  for(i = 0; i < 10; i++) {
    config->in.buffer = (char*)chk_malloc(malloc(513));  // 512 is the maximum line length of the IRC protocol
    config->out.buffer = (char*)chk_malloc(malloc(513));
  }
}

int main(int argc, char** argv) {
  /* Old, single-threaded main
  params* modes = get_modes( argc, argv );

  establish_irc_session(modes);
  char* line;
  char** tokv;
  while (1) {
    line = read_remote(modes->socket_file);
    tokv = parse(line);
    free(line);
    if (tokv != NULL) {
      process_queries(modes, tokv);
      int i;
      for(i = 0; tokv[i] != NULL; i++) {
        free(tokv[i]);
      }
      free(tokv);
    }
  }
  terminate_irc_session(modes->socket_fd);

  free(modes);
  */
  int retval = 0;
  pthread_attr_t* attr = (pthread_attr_t*)chk_malloc(malloc(sizeof(pthread_attr_t)));
  retval = pthread_attr_init(attr);
  if(retval != 0) {
    printf("Unable to initialize pthread_attr: %s\n",strerror(errno));
    exit(1);
  }

  magicbot_config_t* config = (magicbot_config_t*)chk_malloc(malloc(sizeof(magicbot_config_t)));
  init_config(config);
  /* parse config file here */

  /* create a thread to handle lookups, and store its id in addr struct */
  /* create a thread to handle network i/o, and store its id in addr struct*/
  /* create a thread to spawn workers to execute lookups on the database */
  /* create a timekeeping thread to manage flood-control semaphore (optional, pending ratelimiting strategy) */
}
