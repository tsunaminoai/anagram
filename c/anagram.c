#include "./anagram.h"
#include "./trie.h"
#include "./results.h"

int main(int argc, char *argv[]) {

	int min_len = 3;
	int num_res = 20;
	int c;
	char * query;
	
	while( (c = getopt(argc, argv, "hm:n:") ) != -1 )
	{
		switch (c)
		{
			case 'm': min_len = isdigit((int)optarg[0]) ? atoi(optarg) : min_len; break;
			case 'n': num_res = isdigit((int)optarg[0]) ? atoi(optarg) : num_res; break;
			case 'h': print_help(); return 0; break;
			case '?': if(optopt == 'm' || optopt == 'n')
						fprintf(stderr, "Option -%c requires an arument. See help (-h)\n", optopt);
					else if (isprint(optopt))
						fprintf (stderr, "Unknown option `-%c'. See help (-h)\n", optopt);
					else
						fprintf (stderr, "Unknown option character `\\x%x'. See help (-h)\n", optopt);
                    return -1;
                    break;
		}
	}
	if(argc == 1 || argc % 2 != 0 )
	{
		fprintf(stderr,"Too few arguments\n");
		fprintf(stderr,"Usage: %s [-h] [-m min_len] [-n num_results] QUERY\n",BINARY_NAME);
		exit(1);
	}
	
	query = valid_query( argv[argc-1] );

    trie * root;
    root = trie_init();

    FILE * dict = fopen("./words.txt","r");

    if(!trie_dictionary(root,dict))
    {
        fprintf(stderr,"There was an error loading the dictionary\n");
        fclose(dict);
        exit(1);
    }
	fclose(dict);

    bool * mask;
    mask = malloc(sizeof(bool) * strlen(query) );
    memset(mask,1,sizeof(bool) * strlen(query));
    char buffer[128+1] = {'\0'};
    int len = strlen(query);
    
    res_list * reses;
    reses = result_init();
    
    trie_search(root, query, len, mask, buffer, reses);


    result_sort(&reses);
    result_print(reses,min_len,num_res);
    result_free(reses);

    
    trie_free(root);
    free(mask);
    free(query);
    
    return 0;
}


void print_help()
{
	printf("Usage: %s [-h] [-m min_len] [-n num_results] QUERY\n",BINARY_NAME);
	printf("   -m: Minimum length of results. Default: 3\n");
	printf("   -n: Number of results to return. Use 0 for all results. Default: 20\n");
	printf("   -h: This message\n");
	printf("QUERY: String of characters to search within\n");
	printf("\n");
}

char * valid_query( char * query )
{
	int i,j=0;
	
	int len = strlen(query);
	char * vq = malloc( sizeof(char) * ( len + 1 ));
	memset(vq,'\0',sizeof(char) * (len + 1));
	
	for( i=0; i < len; i++)
	{
		if( isalpha((int)query[i]) )
		{
			vq[j] = toupper((int)query[i]);
			j++;
		}
	}
	
	return vq;
}
