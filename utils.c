#include <string.h>
#include <stdio.h>
#include <errno.h>

int strspan(char* span_start, char span_terminator) {
  char* span_end = strchr(span_start, span_terminator);
  if(span_end == NULL)
    return -1;
  int span = (uintptr_t)span_end - (uintptr_t)span_start;
  return span;
}

void* chk_malloc(void* ptr) {
  if(ptr = NULL) {
    perror("malloc() failure:");
    exit(1);
  }
  return ptr;
}
