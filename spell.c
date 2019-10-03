#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
  ssize_t read;
  char * line = NULL;
  size_t len = 0;
  int bucket;
  FILE * fp = fopen(dictionary_file, "r");
  if (fp == NULL) {
    puts("Error opening file");
    return false;
  }

  for (size_t i = 0; i < HASH_SIZE; i++) {
    hashtable[i] = 0;
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    char *pos;
    if ((pos=strchr(line, '\n')) != NULL)
      *pos = '\0';
    bucket = hash_function(line);

    hashmap_t word_node = malloc(sizeof(struct node));
    strncpy(word_node->word, line, LENGTH);
    word_node->word[LENGTH] = 0;
    word_node->next = 0;

    if (!hashtable[bucket]) {
      hashtable[bucket] = word_node;
    } else {
      hashmap_t entry = hashtable[bucket];
      while (entry->next) {
        entry = entry->next;
      }
      entry->next = word_node;
    }
  }
  free(line);
  fclose(fp);

  return true;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
  char word_cpy[LENGTH + 1];
  strncpy(word_cpy, word, LENGTH);
  word_cpy[LENGTH] = 0;
  for (int i = 0; i < strlen(word_cpy); i++) {
    if (word_cpy[i] >= 'A' && word_cpy[i] <= 'Z') {
      word_cpy[i] += 32;
    }
    if (word_cpy[i] < 'a' || word_cpy[i] > 'z') {
      return false;
    }
  }
  int bucket = hash_function(word_cpy);
  if (!hashtable[bucket]) {
    return false;
  } else {
    hashmap_t entry = hashtable[bucket];
    while (entry) {
      if (!strncmp(entry->word, word_cpy, LENGTH)) {
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

bool read_word(FILE* fp, char* word) {
  char c = 0;
  size_t index = 0;
  while ((c = fgetc(fp)) != EOF) {
    if (c == ' ' || c == '\n') {
      word[index] = 0;
      return true;
    } else if (index < LENGTH) {
      word[index++] = c;
    }
  }

  if (index) {
    word[index] = 0;
    return true;
  }

  return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
  int counter = 0;
  char word[LENGTH + 1];
  while (read_word(fp, word)) {
    if (!strlen(word)) {
      continue;
    }

    for (int i = strlen(word); i >= 0; i--) {
      if ((word[i] >= 'a' && word[i] <= 'z') ||
          (word[i] >= 'A' && word[i] <= 'Z')) {
            break;
      } else {
        word[i] = 0;
      }
    }

    if (!check_word(word, hashtable)) {
      misspelled[counter] = malloc(LENGTH + 1);
      strncpy(misspelled[counter], word, LENGTH);
      misspelled[counter++][LENGTH] = 0;
    }
  }

  return counter;
}
