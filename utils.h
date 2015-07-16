#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>

int strspan(char* span_start, char span_terminator);

void* chk_malloc(void* ptr);

int chk_syscall(char* syscall, int retval);

typedef struct {
  char* buffer[10];
  pthread_mutex_t lock;
  sem_t queue;
} buffer_t;

typedef struct {
  char* ip_addr;
  int port;
  int socket_fd;
  int window;
} socket_t;

typedef struct {
  pthread_t input_handler;  // id for thread that will monitor raw IRC channel chatter for queries
  pthread_t output_handler; // id for thread that will handle dumping filled queries to chat
  pthread_t lookup_handler; // id that will spawn workers to search database and fill queries
  pthread_t signal_handler; // id for thread that will handle process signals
  pthread_t timing_handler; // id for thread that handles flood-control timing
} magicbot_threads_t;

typedef struct {
  char* db_file;
  char* botname;
  FILE* socket_file;

  socket_t sock_data;
  buffer_t out;
  buffer_t in;

  magicbot_threads_t thread_ids;
} magicbot_config_t;

#endif
