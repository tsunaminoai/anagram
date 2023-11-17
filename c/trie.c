#include "./trie.h"

trie * trie_init()
{
    trie * t1 = malloc(sizeof(trie));
    t1->word=0;
    t1->used=0;
    memset(t1->nodes,'\0',sizeof(trie*) * NODELIST_SIZE);
    
    return t1;
}

void trie_addword(trie * node, char * word)
{
    int len = strlen(word);
    int i,idx;
    
    for(i=0; i< len; i++)
    {
        idx = ltr2idx(word[i]);
        if(node->nodes[idx] == NULL)
        {
            node->nodes[idx] = trie_init();
        }
        node = node->nodes[idx];
        if(i == len-1)
            node->word = 1;
    }
}

int trie_dictionary(trie* root, FILE * input)
{
    if( input == NULL)
    {
        printf("Error: file pointer is null.");
        return 0;
    }
    char * buff;
    buff = malloc(128 * sizeof(char));
    
    char c;
    int num_words=0;
    
    c = getc(input);
    int count=0;

    while( c != EOF )
    {
        while( c != '\n' && c != '\r' && c != EOF)
        {
            buff[count]=c;
            count++;
            c = getc(input);
        }
        buff[count] = '\0';
        if(count > 0)
        {    
            trie_addword(root,buff);
            //printf("Buff: %s\n",buff);
            num_words++;
            count=0;
        }
        c = getc(input);
    }
    printf("Loaded %d words into tree\n",num_words);
    free(buff);
    return 1;
}

void buff_push(char * buffer, char in)
{
    int len = strlen(buffer);
    buffer[len] = in;
}

void buff_pop(char * buffer)
{
    int len = strlen(buffer);
    buffer[len-1] = '\0';
}

void trie_search(trie * node, char * query, int qlen, bool * mask, char * buffer, res_list * res)
{
    int i, idx;
    
    bool * tmask;
    tmask = calloc(sizeof(bool) , qlen);
    memcpy(tmask,mask,qlen);
    
    for(i=0; i < qlen; i++)
    {
        idx = ltr2idx(query[ i ]);
        if( mask[ i ] && node->nodes[ idx ] && node->nodes[ idx ]->used == 0)
        {
            buff_push(buffer, query[ i ]);
            if( node->nodes[ idx ]->word)
                result_push(res, buffer);
                
            tmask[i] = 0;
            trie_search(node->nodes[ idx ], query, qlen, tmask, buffer, res);
            tmask[i] = mask[i];
            buff_pop(buffer);
            node->nodes[ idx ]->used = 1;
        }
    }
    free(tmask);
}

void trie_free(trie * input)
{
    int i;
    for(i=0; i< NODELIST_SIZE; i++)
    {
        if(input->nodes[i] != NULL)
        {
            trie_free(input->nodes[i]);
        }
    }
    free(input);
}
