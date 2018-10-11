#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



/* Convert STR to a nonnegative integer, storing the result in *OUT.
   STR must be a valid context length argument; report an error if it
   isn't.  Silently ceiling *OUT at the maximum value, as that is
   practically equivalent to infinity for grep's purposes.  */
/*static void
context_length_arg (char const *str, intmax_t *out)
{
  switch (xstrtoimax (str, 0, 10, out, ""))
    {
    case LONGINT_OK:
    case LONGINT_OVERFLOW:
      if (0 <= *out)
        break;
      FALLTHROUGH;
    default:
      die (EXIT_TROUBLE, 0, "%s: %s", str,
           _("invalid context length argument"));
    }
}*/

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
    while((option = getopt(argc,argv, "e:p:a:hc")) != -1){
        switch (option) {
            case 'e':
                //TODO: Busca aproximada
                //Argumento é armazenado em optarg
                printf("optarg = %s\n", optarg);
                break;
            case 'p':
                //TODO: Busca linha por linha no arquivo
                break;
            case 'a':
                //TODO: Busca com um determinado algorithm.
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
