#include "./results.h"

res_list * result_init()
{
    res_list * res = malloc(sizeof(res_list));
    memset(res,0,sizeof(res_list));
    return res;
}

void result_free(res_list * res)
{
	int i;
	for(i=0; i< res->size; i++)
	{
		free(res->results[i]->word);
		free(res->results[i]);
	}
    free(res->results);
	free(res);
}

void result_push( res_list * res, char * word)
{
    int len = strlen(word);
    
    result * new = malloc(sizeof(result));
    new->len = len;
    new->word = malloc(sizeof(char) * (len +1));
    strcpy(new->word,word);
        
    res->size++;
    res->results = realloc(res->results,sizeof(result *) * res->size);
    if( res->results == NULL)
    {
        printf("Error allocating memory in result_push()!\n");
        exit(1);
    }
    
    res->results[ res->size - 1] = new;

}

void result_print(res_list * res, int min, int num)
{
    int i, n, count=0;;
    
    n = (num < res->size && num!=0 ) ? num : res->size;
    
    printf("\nSize\tWord");
    printf("\n----\t----\n\n");
    
    for(i=0; i < n; i++)
    {
    	if( res->results[i]->len >= min )
    	{
	        printf("%d\t%s\n",res->results[i]->len,res->results[i]->word);
	        count++;
	    }
    }
    printf("\nFound %d words. Showing %d.\n",res->size,count);

}

result * result_copy(result * in)
{
	result * out = malloc(sizeof(result));
	out->len = in->len;
	out->word = malloc( sizeof(char) * in->len + 1);
	strcpy(out->word, in->word);
	
	return out;
}

void result_dup(res_list ** input)
{
	res_list * res = *(input);
	res_list * new = result_init();
	new->size = 0;
	new->results = malloc(sizeof(result*));
	
	int i;
	char buffer[128] = {'\0'};
	
	for( i=0; i< res->size; i++)
	{
		if( strcmp(res->results[i]->word, buffer) != 0)
		{
			new->results = realloc(new->results, sizeof(result*) * (new->size+1));
			new->results[new->size] = result_copy(res->results[i]);
			new->size++;
			strcpy(buffer,res->results[i]->word);
		}

	}
	
	*input = new;
	result_free(res);

}


int res_cmp(const void *a, const void *b)
{
	result ** ia = (result **)a;
    result ** ib = (result **)b;
    
    if( (*ib)->len < (*ia)->len )
    	return -1;
    else if ( (*ib)->len > (*ia)->len )
    	return 1;
    
    if ( strcmp((*ia)->word, (*ib)->word) < 0 )
    	return -1;
    else if ( strcmp((*ia)->word, (*ib)->word) > 0 )
    	return 1;
    else
    	return 0;
}

void result_sort(res_list ** res)
{
    qsort( (*res)->results, (*res)->size , sizeof(result*), res_cmp);
    //result_dup( res );
}
