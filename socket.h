struct node {
  char* data;
  struct node* next;
};

void establish_irc_session( struct params* modes );
int strspan(char* span_start, char span_terminator );
