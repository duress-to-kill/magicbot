#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "jsmn.h"
#include "json_manager.h"
#include "magicbot.h"
/* TESTING ZONE */

static card *table[tablesize];

static char *raw = NULL;
static int rawlen = 0;
static jsmn_parser pos;
static jsmntok_t *tok = NULL;
static size_t toklen = 1000000;
static jsmnerr_t obj_count = 0;

static int parse_raw(const char* raw, const char *src_filename) {
    jsmn_init(&pos);
  do {
    if ((tok = realloc(tok, toklen * sizeof(jsmntok_t))) == NULL) {
      fprintf(stderr, "Error: failed to allocate %ld indexes (%ld bytes) for json parsing: realloc(): %s\n", (long)toklen, (long)toklen * (long)sizeof(jsmntok_t), strerror(errno));
      return 1;
    }
    if((obj_count = jsmn_parse(&pos, raw, rawlen, tok, toklen)) == JSMN_ERROR_NOMEM) {
      printf("%ld was not enough bytes, adding more...\n", (long)toklen);
      toklen += 25000;
    } else if (obj_count == JSMN_ERROR_INVAL) {
      fprintf(stderr, "Error: json source file failed strict syntax checking.");
      return 1;
    } else if (obj_count == JSMN_ERROR_PART) {
      fprintf(stderr, "Error: json source file is incomplete.\n");
      return 1;
    }
  } while (obj_count == JSMN_ERROR_NOMEM);

  printf("Reinitialized card database from %s: Loaded %ld objects, %ld bytes (%ld indexes allocated)\n", src_filename, (long)obj_count, (long)rawlen, (long)toklen);
  return 0;
}

int load_cards_from_file(const char *src_filename) {
  int ret;

  FILE *src_file = fopen(src_filename, "r");
  if ((src_file = fopen("./AllCards-x.json", "r")) == NULL ) {
    perror("Error reading json source file: fopen()");
    return 1;
  }

  if ((ret = fseek(src_file, 1, SEEK_END)) == -1 || (rawlen = ftell(src_file)) == -1 ) {
    perror("Error getting file length: fseek()");
    return 1;
  }
  rewind(src_file);

  if ((raw = realloc(raw, rawlen + 1 * sizeof(char))) == NULL) {
    fprintf(stderr, "Error: failed to allocate %d bytes for source file: realloc(): %s\n", rawlen + 1, strerror(errno));
    return 1;
  }

  if (fgets(raw, rawlen, src_file) != raw) {
    perror("Error while reading source file: fgets()");
    return 1;
  }

  ret = parse_raw(raw, src_filename);
  // this return value should probably be checked.

  // call handler to rebuild hash table
  // update_table();

  return 0;
}

static unsigned int hash(const char *key, int len)
{
  // The apparently well-known ELF hash
  // cut and paste courtesy of http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx

  //unsigned char *p = key;
  unsigned h = 0, g;
  int i;

  for ( i = 0; i < len; i++ ) {
    h = ( h << 4 ) + key[i];
    g = h & 0xf0000000L;

    if ( g != 0 )
      h ^= g >> 24;

    h &= ~g;
  }

  return h;
}

static int insert_card(const jsmntok_t *card_obj) {
  int i = 0;

  if (card_obj == NULL) {
    fprintf(stderr, "Error: Tried to insert a null card into hash table\n");
    return 1;
  }

  char *start = strstr(raw + card_obj->start, "\"name\":\"");
  if (start == NULL) {
    fprintf(stderr, "Error: found malformed card object at byte offset %ld during indexing\n", (long)card_obj - (long)tok);
    return 1;
  }
  start += 8;
  char buffer[50];
  strncpy(buffer, start, 49);
  int len = strspan(buffer, '\"');
  if (len < 1) {
    fprintf(stderr, "Error: found malformed card object at byte offset %ld during indexing\n", (long)card_obj - (long)tok);
  }
  start[len] = '\0';

  char name[len + 1];
  strncpy(name, start, len + 1);

  for(i = 0; i <= len; i++) {
    name[i] = tolower(name[i]);
  }

  card* current = table[hash(name, strlen(name)) % tablesize];
  // TESTING

  printf("Would insert card \"%s\", found at byte offset %ld\n", buffer, (long)start - (long)tok);

  return 0;
}

static int update_table() {
  /* This should be a general handler that is able to determine what's already in the hash table and only adding the difference. */
  /* mtgjson.com seems to produce objects sorted by set, then alphabet. Thus, new cards ought to be appended to the file, and it should be possible to seek backwards, using strcmp() to break on set boundaries, and determine where to start adding. */
  /* Addendum: this may not actually be true. Probably just best to do a full sweep across the list and insert everything. */
  return 0;
}

void init_table() {
  int i;
  card *current, *prev;;
  for (i = 0; i < tablesize; i++) {
    if (table[i] != NULL) {
      current = table[i];
      while(current != NULL) {
        prev = current;
        free(prev);
      }
      table[i] = NULL;
    }
  }
}

/* TEST FRAMEWORK BELOW THIS POINT */

void cleanup() {
  free(raw);
  free(tok);
}

/*
int main(int argc, char* argv[]) {
  load_cards_from_file(argv[1]);

  insert_card(&tok[atoi(argv[2])]);
  cleanup();
  return 0;
}
*/
