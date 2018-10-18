#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <string>

#include "ahoCohasick.h"

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

static std::vector<std::string> alg_array =
    {"boyermoore", "ahocorasik", "e1", "e2"};

/*Usage function to show messages*/
void usage(std::string const& name, bool status){
    if(!status){
        std::cout << "Usage: " << name
        << " [OPTION]... PATTERN TEXTFILE [TEXTFILE...]" <<'\n';
        std::cout << "Try '" << name
        << " --help' for more information"<<'\n';
        exit(EXIT_FAILURE);
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

/*Function to set the index of first wildcard argument or textfile*/
void set_txt_index(int opt, int& index){
    index = index + opt;
}

/*Caso seja uma string, anexo a um vetor de string (único)*/
void set_pat(std::vector<std::string> &pat_array,
  std::string str,bool pflag){

  if(!pflag)
    pat_array.push_back(str);

}

/*Teste de validade, argumentos devem ser no minimo 3
o primeiro textfile deve ser válido*/
void check_args(int argc, int txt_index, std:: string const& prog ){
    if(argc < 3)
        usage(prog,false);

    if(argc == txt_index)
        usage(prog,false);

}

/*o ultimo argumento deve ser um tipo texto*/
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

void set_emax(std::string const& str, int &emax){
    emax = std:: stoi(str);
    //std::cout << emax << '\n';
}

/*Convert the input file in array of strings*/
void build_string_array(char* file,
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

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
/*for testes*/
void highlight_match(std::string line, std::string match, int first){
    int j = 0;
    for(int i = 0; i < line.length() ; i++){
        if(i == first && first <= match.length()){
            //std::cout << "chega?" << '\n';
            std::cout << RED << line.at(i) << RESET;
            first++;

        }
        else{
          std::cout << RESET << line.at(i);
        }
    }
    std::cout << '\n';

}
/*for tests*/
void simple_search(char* file , std::vector<std::string> pat_array,
    bool cflag){

    int find_result = 0;
    for(int i = 0; i<pat_array.size(); i++){

        // std::cout << "string é --" << pat_array[i] << "--" << '\n';
        std::ifstream infile(file);
        std::string line;
        while(std::getline(infile,line)){
            if(line.find(pat_array[i]) != -1){
                if(!cflag){
                    std::cout << RED << line << RESET <<'\n';
                    // highlight_match(line, pat_array[i],find);
                }
                find_result++;
            }

        }
        //std::cout   << '\n';
        infile.close();

    }
    if(cflag)
        std::cout << find_result << '\n';


}


bool aho_aux(std::vector<std::vector<int>> array){
    int occ = 0;
    for(int i =0; i< array.size(); i++){
        occ += array[i].size();
        if(occ > 0)
            return true;
    }

    return false;

}

void aho_call(char* file, std::vector<std::string> pat_array,
    bool cflag){
        std::ifstream infile(file);
        std::string line;
        // std::string ab = "abcdefghijklmnopqrstuvwxyz \n\0\r";
        std::string ab;
        int find_result = 0;
        for(int i =0; i<256;i++){
            char a = i;
            ab += a;
        }

        AhoCohasick aho;
        while(std::getline(infile,line)){
            // std::cout << line << '\n';
            std::vector<std::vector<int> > array = aho.ahocohasick(line,pat_array,ab);
            if(aho_aux(array)){
                if(!cflag)
                    std::cout << line << '\n';
                find_result++;
            }
        }
        if(cflag)
            std::cout << find_result << '\n';
        infile.close();
    }
/*Chamada dos algoritmos, incluindo a cflag
Deve ser a chamada final da função call_pmt*/
void call_alg(std::string alg_name, char* file,
   std::vector<std::string> pat_array, int emax, bool cflag){

  if(alg_name.compare(alg_array[0]) == 0){
    //TODO: BoyerMoore
    // std::cout << alg_name << '\n';
    simple_search(file, pat_array, cflag);
  }
  else if(alg_name.compare(alg_array[1]) == 0){
    //TODO: Upgrade ahocorasik
    std::cout << alg_name << '\n';
    aho_call(file,pat_array,cflag);
  }
  else if(alg_name.compare(alg_array[2]) == 0){
    //TODO: e1 parametro emax
    std::cout << alg_name << '\n';
  }
  else if(alg_name.compare(alg_array[3]) == 0){
    //TODO: e2 parametro emax
    std::cout << alg_name << '\n';
  }
  // infile.close();
}

/*Controller for call the algorithms*/
/* Cada linha deve ser impressa apenas uma vez, independente
da quantidade de ocorrencias que ela contenha.*/
void call_pmt(std::string alg_name, char* file,
   std::vector<std::string> pat_array, int emax,  bool cflag){
     //TODO: Defaults e Sugestões
      call_alg(alg_name, file, pat_array , emax, cflag);
    }

int main(int argc, char *argv[]) {
    int option;
    bool eflag, pflag, aflag, cflag = false;
    int txt_index = 2;
    int emax = -1;
    std::string alg_name;
    std::vector<std::string> pat_array;

    check_args(argc, txt_index, argv[0]);
    check_file(argv[argc-1], argv[0]);

    while((option = getopt_long(argc,argv, "e:p:a:ch",
        long_options, NULL)) != -1){
            switch(option){
                case 'e':
                    eflag = true;
                    set_emax(optarg, emax);
                    //TODO: Busca aproximada PRIORITY 1
                    set_txt_index(2,txt_index);
                    break;
                case 'p':
                    pflag = true;
                    /*TODO: Busca linha por linha no arquivo
                    PRIORITY 2*/
                    build_string_array(optarg, pat_array);
                    set_txt_index(1,txt_index);
                    break;
                case 'a':
                    /*TODO: Busca com um determinado algorithm.
                    {BoyerMoore, AhoCorasik, +2 Aprox} PRIORITY 3 */
                    aflag = true;
                    alg_name = optarg;
                    set_txt_index(2,txt_index);
                    break;
                case 'c':
                    cflag = true;
                    set_txt_index(1,txt_index);
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
        //txt_index -> first textfile
        //txt_index-1 -> string
        check_args(argc, txt_index, argv[0]);

        set_pat(pat_array, argv[txt_index-1],pflag);

        if(pat_array.size() == 1){
            //é uma string
            std::cout << "é uma string --" << argv[txt_index-1] << "--"<<'\n';
        }
        else{
            //vetor de strings
            std::cout << "é um vetor de string - " << pat_array[0]<<'\n';
        }
        call_pmt(alg_name, argv[txt_index], pat_array, emax, cflag);



    return 0;
}
