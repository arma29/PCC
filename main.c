#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
#define MAX_FILE_NAME 100

int main(int argc, char **argv) {
    //TODO: Initial variables
    int option;
    int default_context = -1;
    int first_index = 0;
    while((option = getopt(argc,argv, "e:p:a:hc")) != -1){
        switch (option) {
            case 'e':
                //TODO: Busca aproximada
                if(optind > first_index)
                    first_index = optind+1;
                break;
            case 'p':
                //TODO: Busca linha por linha no arquivo
                //Argumento é armazenado em optarg
                printf("optarg = %s\n", optarg);
                //Index do argumento é armazenado em optind
                printf("currently arg is = %d\n", optind);
                if(optind > first_index)
                    first_index = optind+1; //utilitario para o while ou for.
                    /*for(i = first_index; i < argc; i++){
                        FILE *aux = fopen(argv[i], "r");

                    }*/
                //Testes , optopt = é o char ,
                printf("optopt vale = %d // opterr vale = %d\n", optopt, opterr);
                break;
            case 'a':
                //TODO: Busca com um determinado algorithm.
                if(optind > first_index)
                    first_index = optind+1;
                break;
            case 'c':
                //TODO: Show a total count of every match pattern-file
                printf("deve mostrar a conta\n");
                break;
            default:
                //TODO: Show a help function listin all algorithms
                fprintf(stderr, "getopt\n");
        }
    }
    printf("first index vale = %d\n", first_index);
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
    }

    return 0;
}
