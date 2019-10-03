#include <stdlib.h>
#include "dictionary.h"


int main(int argc, const char* argv[]) {
  if (argc != 3) {
    puts("Wrong number of args");
    return 1;
  }

  hashmap_t hashtable[HASH_SIZE];
  load_dictionary(argv[2], hashtable);

  FILE * fp = fopen(argv[1], "r");
  if (fp == NULL) {
    puts("Error opening file");
    return false;
  }
  char** misspelled = malloc(sizeof(char*) * MAX_MISSPELLED);
  int num_misspelled = check_words(fp, hashtable, misspelled);
  for (size_t i = 0; i < num_misspelled; i++) {
    puts(misspelled[i]);
    free(misspelled[i]);
    misspelled[i] = 0;
  }

  free(misspelled);
  misspelled = 0;
  for (size_t i = 0; i < HASH_SIZE; i++) {
    if (hashtable[i]) {
      hashmap_t entry = hashtable[i];
      hashmap_t tmp;
      while (entry) {
        tmp = entry;
        entry = entry->next;
        tmp->next = 0;
        free(tmp);
      }
      hashtable[i] = 0;
    }
  }
  fclose(fp);

  return 0;
}
