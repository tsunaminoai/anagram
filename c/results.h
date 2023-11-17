#ifndef _RESULTS
#define _RESULTS

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int len;
    char * word;
} result;

typedef struct {
    
    int size;
    result ** results;
    
} res_list;


res_list * result_init();
void result_push( res_list *, char *);
void result_sort(res_list **);
void result_print(res_list *, int, int);
void result_free(res_list *);
#endif
