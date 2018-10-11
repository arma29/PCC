#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>

/*
Busca Exata (default).

-e, --edit emax: Localiza todas as ocorrencias aproximadas
do padrão a uma distãncia de edição máxima emax.

-p, --patern patternfile: Realiza a busca de todos os
padroes contidos no arquivo  patternfile.

-a, --algorithm algorith_name: Realiza a busca de padrões
usando o algoritmo.

-h, --help: Lista algoritmos. (2 pra exata e 2 pra aprox)

-c, --count: Quantidade total de ocorrências do(s) padrão(ões)
contidas no(s) arquivo(s)de texto. (ou seja apenas um número)

*/

/*Necessary for getopt_long, use as argument 4*/
static struct option const long_options[]=
{
    {"help", no_argument, NULL, 'h'},
    {"count", no_argument, NULL, 'c'},
    {"edit", required_argument, NULL, 'e'},
    {"pattern", required_argument, NULL, 'p'},
    {"algorithm", required_argument, NULL, 'a'},
    {0,0,0,0}, //end of array
};

void usage (char *name, int status){
    if(status == 0){
        fprintf(stderr,"Usage: %s [options] pattern textfile [textfile...]\n", name);
        fprintf(stderr, "Try '%s -h' for more information.\n", name);
    }
    else{
        printf("Usage: %s [OPTION]... PATTERN TEXTFILE [TEXTFILE...]\n", name);
        printf("Search for PATTERN in each FILE.\n");
        printf ("\
Example: %s -e 4 'hello world' menu.h main.c\n\
\n\
Pattern selection and interpretation:\n", name);
        printf("\
    -e, --edit=EMAX         Find the pattern matches with a max edit distance\n\
    -p, --pattern=PFile     Make the search of all patterns in patternfile\n\
    -a, --algorithm=AName   Make the pattern search with the algorithm\n\
    -c, --count             Print the total count of matches\n\n");
            printf("\
Miscellaneous:\n\
    -h, --help              Display this help text and exit\n");
    }

    exit(-1);
}
#define MAX_FILE_NAME 100
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

int Search_in_File(char *fname, char *str) {
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    
    //If fname is null -> string
    if((fp = fopen(fname, "r")) == NULL) {
      printf("maybe string\n");
      return(-1);
    }

    while(fgets(temp, 512, fp) != NULL) {
        if((strstr(temp, str)) != NULL) {
            //printf("A match found on line: %d\n", line_num);
            //TODO: make a good color
            printf("%s", temp);
            int i;
            int j = 0;
            for(i=0;i<strlen(temp);i++){
                if(temp[i] = str[j]){
                    printf(RED "%c" RESET, temp[i]);
                    j++;
                }
                else{
                    printf(RESET "--%c--" RESET, temp[i]);
                }
            }
            printf("\n");
            //printf("%s\n", temp);
            //printf("%s\n", str);
            
            find_result++;
        }
        line_num++;
    }

    if(find_result == 0) {
        printf("\nSorry, couldn't find a match.\n");
    }
    
    //Close the file if still open.
    if(fp) {
        fclose(fp);
    }
    return(0);
}

int main(int argc, char **argv) {
    //TODO: flags
    int option;
    int first_index = 0;
    if(argc < 2)
        usage(argv[0],0);
    while((option = getopt_long(argc,argv, "e:p:a:ch", long_options, NULL)) != -1){
        switch (option) {
            case 'e':
                //TODO: Busca aproximada
                if(optind > first_index)
                    first_index = optind+1;
                break;
            case 'p':
                //TODO: Busca linha por linha no arquivo
                if(optind > first_index)
                    first_index = optind; //utilitario para o while ou for.

                //TODO: Organize as function
                //For Wildcards
                int i;
                int j = 1;
                for(i=first_index; i<argc;i++){
                    //printf("hello?\n");
                    FILE *aux;
                    if( (aux = fopen(optarg, "r")) == NULL ){
                        printf("error\n");
                        return -1;
                    }
                    printf("wildcard number %d\n", j);
                    char line[128];
                    while(fgets(line, sizeof(line), aux) != NULL){
                        line[strlen(line) - 1] = '\0';
                        //printf("line é --%s--\n",line);
                        
                        Search_in_File(argv[i], line);
                    }
                    fclose(aux);
                    j++;
                }

                break;
            case 'a':
                //TODO: Busca com um determinado algorithm.
                if(optind > first_index)
                    first_index = optind+1;                
                break;
            case 'c':
                //TODO: Show a total count of every match pattern-file "namefile": count
                break;
            case 'h':
                //TODO: void usage
                usage(argv[0],1);
                break;  

            default:
                return -1;
        }
    }

    return 0;
}
