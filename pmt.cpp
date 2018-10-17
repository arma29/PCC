#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <string>

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

/*Usage function to show messages*/
void usage(std::string const& name, bool status){
    if(!status){
        std::cout << "Usage: " << name
        << " [OPTION]... PATTERN TEXTFILE [TEXTFILE...]" <<'\n';
        std::cout << "Try '" << name
        << " --help' for more information"<<'\n';
    }
    else{
        std::cout << "Usage: " << name
        << " [OPTION]... PATTERN TEXTFILE [TEXTFILE...]" <<'\n';
        std::cout << "Search for PATTERN in each FILE" << '\n';
        std::cout << "\
Example: "<< name << " -e 4 'hello world' menu.h main.c\n" << '\n';

        std::cout <<"\
Pattern selection and interpretation:\n\
    -e, --edit=EMAX         Find the pattern matches with a max edit distance\n\
    -p, --pattern=PFile     Make the search of all patterns in patternfile\n\
    -a, --algorithm=AName   Make the pattern search with the selected algorithm\n\
    -c, --count             Print the total count of matches\n\n" << '\n';

        std::cout << "\
Miscellaneous:\n\
    -h, --help              Display this help text and exit\n\n" << '\n';
        std::cout << "\
Algorithm Names:\n\
    boyermoore              Default Algorithm for search\n\
    ahocorasik              Default Algorithm for multi string search\n\
    e1                      Default Algorithm for fuzzy search\n\
    e2                      Default for...\n\n" << '\n';

        std::cout << "\
Relate the bugs to: arma@cin.ufpe.br // mapa@cin.ufpe.br\n\
PMT Page: <http://www.github.com/arma29/PCC>\n" << '\n';
    }

    exit(EXIT_SUCCESS);
}

/*Function to set the index of last non wildcard argument*/
void set_txt_index(int opt, int& index){
    if(opt > index){
        index = opt+1;
    }
}

void check_args(int argc, std:: string const& prog ){
    if(argc < 3)
        usage(prog,false);
}

void check_file(char *file,
    std::string const& prog){

    std::ifstream infile(file);
    if(!infile){
        std::cout << prog << ": '" << file
        << "' File or Directory not found" << '\n';
        exit(EXIT_FAILURE);
    }
    // std::string line;
    // while(std::getline(infile,line)){
    //     std::cout << line <<'\n';
    //     std::cout << line.length() << '\n';
    // }
    infile.close();
}

void check_edit(std::string const& str, int &emax){
    emax = std:: stoi(str);
    //std::cout << emax << '\n';
}

void build_string_array(char *file,
    std::vector<std::string> &str_array){

    std:: ifstream infile(file);
    if(!infile){
        std::cout << "File or Directory not found" << '\n';
        exit(EXIT_FAILURE);
    }
    std::string line;
    while(std::getline(infile,line)){
        str_array.push_back(line);
    }

    infile.close();
}

int main(int argc, char *argv[]) {
    int option;
    int eflag, pflag, aflag, cflag = 0;
    int txt_index = 0;
    int emax;

    check_args(argc, argv[0]);
    check_file(argv[argc-1], argv[0]);

    std::vector<std::string> pat_array;

    while((option = getopt_long(argc,argv, "e:p:a:ch",
        long_options, NULL)) != -1){
            switch(option){
                case 'e':
                    eflag = 1;
                    check_edit(optarg, emax);
                    //TODO: Busca aproximada PRIORITY 1
                    set_txt_index(optind,txt_index);
                    break;
                case 'p':
                    pflag = 1;
                    /*TODO: Busca linha por linha no arquivo
                    PRIORITY 2*/
                    build_string_array(optarg, pat_array);
                    set_txt_index(optind,txt_index);
                    break;
                case 'a':
                    /*TODO: Busca com um determinado algorithm.
                    {BoyerMoore, AhoCorasik, +2 Aprox} PRIORITY 3 */
                    aflag = 1;
                    set_txt_index(optind,txt_index);
                    break;
                case 'c':
                    cflag = 1;
                    /*TODO: Show a total count of every match pattern-file
                    "namefile": count MAX_PRIORITY 4*/
                    break;
                case 'h':
                    usage(argv[0],true);
                    break;
                default:
                    return -1;
            }
        }
        //txt_index-1 te dará a pattern
        if(pat_array.empty()){
            //é uma string
            std::cout << "é uma string - " << argv[txt_index-1] <<'\n';
        }
        else{
            //vetor de strings
            std::cout << "é um vetor de string - " << pat_array[0]<<'\n';
        }
    return 0;
}
