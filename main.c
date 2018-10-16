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
#define MAX_WORD_LENGTH	128
#define MAX_LINES 128
#define ALGORITHM_NUMBER 4
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

int build_strarray(char *filename, char* strings[]){
    FILE *fp;
    char line[MAX_WORD_LENGTH];
    if((fp=fopen(filename,"r")) == NULL){
        printf("couldn't open the file\n");
        return -1;
    }

    int k = 0;
    while(fgets(line, sizeof(line), fp) != NULL){
        line[strlen(line) - 2] = '\0';
        // line[strlen(line) - 2] = '\0';
        // strings[k] = strdup(line);
        //printf("line tem %d\n", (int)strlen(line));
        strings[k] = malloc(sizeof(line));
        strcpy(strings[k],line);
        k++;
    }

    return k;

    fclose(fp);


}

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
        // printf("%d -- %d\n", (int)strlen(temp), (int)strlen(str));
        if((strstr(temp, str)) != NULL) { //one per line
            //printf("A match found on line: %d\n", line_num);
            //TODO: make a good color
            int i;
            int j = 0;
            for(i=0;i<strlen(temp);i++){

                if(temp[i] == str[j]){
                    printf(RED "%c" RESET, temp[i]);
                    j++;
                }
                else{
                    printf("%c", temp[i]);
                }

            }
            find_result++;
        }
        line_num++;
    }
    printf("find_result = %d\n", find_result);
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
    int eflag, pflag, aflag, cflag = 0;
    int option;
    int first_index = 0;
    char *strarray[50];
    char *algorithm_names[ALGORITHM_NUMBER] = {"boyermoore", "ahocorasik", "e1", "e2"};
    if(argc < 2)
        usage(argv[0],0);
    while((option = getopt_long(argc,argv, "e:p:a:ch", long_options, NULL)) != -1){
        switch (option) {
            case 'e':
                eflag = 1;
                //TODO: Busca aproximada
                //A busca aproximada deve receber como argumento obrigatorio o erro maximo - optarg
                if(optind > first_index)
                    first_index = optind+1;
                break;
            case 'p':
                pflag = 1;
                //TODO: Busca linha por linha no arquivo
                if(optind > first_index)
                    first_index = optind; //utilitario para o while ou for.

                int i;
                for(i=first_index; i<argc;i++){
                    //printf("hello?\n");

                    printf("wildcard number %d - %s\n", (i+1-first_index), argv[i]);


                    int lines = build_strarray(optarg,strarray);
                    int j;
                    for(j = 0; j < lines;j++)
                        Search_in_File(argv[i],strarray[j]);
                    //printf("%s ", strarray[0]);

                }

                break;
            case 'a':
                aflag = 1;
                //TODO: Busca com um determinado algorithm. {BoyerMoore, AhoCorasik, +2 Aprox} PRIORITY 3
                if(optind > first_index)
                    first_index = optind+1;

                for(i=0;i<ALGORITHM_NUMBER;i++){
                    if(strcmp(algorithm_names[i],optarg) == 0){
                        printf("choosens --%s--\n", algorithm_names[i]);
                        break;
                    }
                }
                printf("test\n");

                break;
            case 'c':
                cflag = 1;
                //TODO: Show a total count of every match pattern-file "namefile": count MAX_PRIORITY 4
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
