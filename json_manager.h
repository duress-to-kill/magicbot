#define tablesize 5000

// A hash table node for indexing card objects in parsed json data
//typedef struct card Card;

typedef struct {
  char *name; // The english name of the corresponding card
  unsigned int index; // The array index of the jsmntok_t struct that points to this card
  //card* next;
  void *next;
} card;
