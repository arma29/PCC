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
            printf("A match found on line: %d\n", line_num);
            printf("\n%s\n", temp);
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
                //Argumento é armazenado em optarg
              //  printf("optarg = %s\n", optarg);
                //Index do argumento é armazenado em optind
                //printf("currently arg is = %d\n", optind);
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
                        printf("line é --%s--\n",line);
                        
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
                /*FILE *aux;
                if((aux = fopen(argv[argc-2], "r")) == NULL){
                    printf("string\n");
                    Search_in_File(argv[argc-1], argv[argc-2]);
                }
                //TODO: organizar para -f
                else{
                    printf("arquivo\n");
                    char line[128];
                    while(fgets(line, sizeof line, aux) != NULL){
                        
                        line[strlen(line) - 1] = '\0'; //ASsim corta-se a fim de linha
                        printf("line é --%s--\n",line);
                        Search_in_File(argv[argc-1], line);
                    }
                }
                //TODO wildcards
                if((argc - first_index) > 1) {
                    //We have wildcards
                    for(i=first_index; i<argc;i++){
                        Search_in_File(argv[i], argv[first_index]);
                    }
                }

                if(aux)
                    fclose(aux);*/
                
                break;
            case 'c':
                //TODO: Show a total count of every match pattern-file
                printf("deve mostrar a conta\n");
                break;
            case 'h':
                //TODO: void usage
                usage(argv[0],1);
                break;  

            default:
                return -1;
        }
    }
    //printf("first index vale = %d\n", first_index);
    //printf("argc vale %d\n", argc);
    /*
    //TODO: A Search pattern-file program.
    FILE *fp;
    //printf("Default here\n");
    //char filename[MAX_FILE_NAME] = argv[2];
    //TODO: um loop sobre o Wildcards
    fp = fopen(argv[argc-1], "r");

    if(fp == NULL){
        printf("Could not open file\n");
        return 0;
    }
    //Wildcards se expandem em outros argumentos.
    printf("argc vale = %d\n", argc);
    //Read Lines
    char line[128];
    //Each line will be a string.
    char file[10][128];
    int i = 0;
    while(fgets(line, sizeof line, fp) != NULL){
        strcpy(file[i], line); //This way store line-string
        fputs(line,stdout);
        i++;
    }*/

    return 0;
}
