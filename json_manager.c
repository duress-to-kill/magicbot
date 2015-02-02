#include "jsmn.h"
/* TESTING ZONE */

static char *raw = NULL;
static int rawlen = 0;
static jsmn_parser json;
static jsmntok_t *json_tok;
static size_t tok_count;

int init_from_file() {

}
