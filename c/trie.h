#ifndef _TRIE
#define _TRIE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "./results.h"

#define NODELIST_SIZE 26
typedef struct _trie {
    bool word;
    bool used;
    struct _trie * nodes[NODELIST_SIZE];
} trie;

#define ltr2idx(X) (X-65)

trie * trie_init();
void trie_free(trie *);
void trie_addword(trie *, char *);
int trie_dictionary(trie*, FILE *);
void trie_search(trie *, char *, int, bool *, char *, res_list * );

#endif
