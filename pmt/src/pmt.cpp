#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <string>

#include "ahoCohasick.h"
#include "boyerMoore.h"
#include "sellers.h"
#include "wumanber.h"

/*If you open the command palette, using Ctrl+Shift+p,
you should find a fold all function by typing “fold all”.
A shortcut for fold all is Ctrl+Alt+Shift+[.*/
/*fold at ident level 1 - ctrl+K ctrl+1*/

/*Necessary for getopt_long, use as argument 4*/
static struct option const long_options[]=
{
	{"help", no_argument, NULL, 'h'},
	{"count", no_argument, NULL, 'c'},
	{"edit", required_argument, NULL, 'e'},
	{"pattern", required_argument, NULL, 'p'},
	{"algorithm", required_argument, NULL, 'a'},
	{0,0,0,0},     //end of array
};

static std::vector<std::string> alg_array =
{"bm", "aho", "sel", "wu"};

/*Usage function to show messages*/
void usage(std::string const& name, bool status)
{
	if(!status) {
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
    bm, BoyerMoore          Default Algorithm for search\n\
    aho, AhoCorasick        Default Algorithm for multi string search\n\
    sel, Sellers            Default Algorithm for fuzzy search\n\
    wu, Wumanber            Default for...\n\n" << '\n';

		std::cout << "\
Relate the bugs to: arma@cin.ufpe.br // mapa@cin.ufpe.br\n\
PMT Page: <http://www.github.com/arma29/PCC>\n" << '\n';
	}

	exit(EXIT_SUCCESS);
}

//TODO: delete
void print_edit(std::vector<int> edit){
	std::cout << "[";
	for(auto i : edit) {

		std::cout << i << ",";
	}
	std::cout << "]" << '\n';
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

	std::ifstream infile(pat_array[0]);

	if(infile.is_open()){
		std::cout << "No file" << '\n';
		usage("./pmt", false);
	}
}

/*Teste de validade, argumentos devem ser no minimo 3
   o primeiro textfile deve ser válido*/
void check_args(int argc, int txt_index, std:: string const& prog){

	if(argc < 3)
		usage(prog,false);

	if(argc == txt_index)
		usage(prog,false);

}

/*o ultimo argumento deve ser um tipo texto*/
void check_file(char *file,
                std::string const& prog){

	std::ifstream infile(file);
	if(!infile) {
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
	if(!infile) {
		std::cout << "File or Directory not found" << '\n';
		exit(EXIT_FAILURE);
	}
	std::string line;
	while(std::getline(infile,line)) {
		str_array.push_back(line);
	}
	infile.close();
}

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
/*for testes*/
void highlight_match(std::string line, std::string match, int first){
	int j = 0;
	for(int i = 0; i < line.length(); i++) {
		if(i == first && first <= match.length()) {
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
void simple_search(char* file, std::vector<std::string> pat_array,
                   bool cflag){

	int find_result = 0;
	for(int i = 0; i<pat_array.size(); i++) {

		// std::cout << "string é --" << pat_array[i] << "--" << '\n';
		std::ifstream infile(file);
		std::string line;
		while(std::getline(infile,line)) {
			if(line.find(pat_array[i]) != -1) {
				if(!cflag) {
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

int aho_aux(std::vector<std::vector<int> > array){

	int occ = 0;
	for(int i =0; i< array.size(); i++) {
		occ += array[i].size();

	}

	return occ;

}

void aho_call(char* file, std::vector<std::string> pat_array,
              bool cflag){
	std::ifstream infile(file);
	std::string line;
	std::string ab;
	for(int i =0; i<128; i++) {
		char a = i;
		ab += a;
	}

	Aho::G g; std::vector<int> f;Aho::O o;
	std::tuple<Aho::G, std::vector<int>, Aho::O> ret = Aho::build_fsm(pat_array, ab);
	std::vector<int> patSize(pat_array.size());
	for (int i = 0;i < pat_array.size();i++) {
		patSize[i] = pat_array[i].size();
	}
	int count = 0;
	int lines = 0;
	while(std::getline(infile,line)) {
		// std::cout << line << '\n';
		std::vector<std::vector<int> > array =
		    Aho::ahocohasick(line,patSize,ret);
		int find = aho_aux(array);
		if(find > 0) {
			if(!cflag)
				std::cout << line << '\n';
			lines++;
			count += find;
		}
	}
	if(cflag)
		std::cout << "[Lines - " << lines << "] , [Count - "
		<< count << "], AhoCorasick" <<'\n';
	infile.close();
}

void boy_call(char* file, std::string pat,
              bool cflag){

	std::ifstream infile(file);
	std::string line;
	std::string ab;
	for(int i =0; i<128; i++) {
		char a = i;
		ab += a;
	}

	std::map<char, int> C = Bm::bad_char(pat, ab);
	std::vector<int> S = Bm::good_suffix(pat);

	int count = 0;
	int lines = 0;
	while(std::getline(infile,line)) {
		std::vector<int> array = Bm::boyer_moore(line,pat,ab.length(),C,S);
		if(!array.empty()) {
			if(!cflag)
				std::cout << line << '\n';
			lines++;
			count += array.size();
		}
	}
	if(cflag)
		std::cout << "[Lines - " << lines << "] , [Count - "
		<< count << "], BoyerMoore" <<'\n';
	infile.close();


}



int aprox_count(std::vector<int> occ){
	int count = 0;
	for (size_t i = 0; i < occ.size()-1; i++) {
		/* code */
		if((occ[i+1] - occ[i]) > 1){
			count++;
		}

		if(occ[i+1] == (occ[occ.size()-1])){
			count++;
		}
	}

	return count;
}

void sel_call(char *file, std::string pat, int emax, bool cflag){
    //TODO: Function and sel_aux , that returns the count, {if(prox-atual > 1)count++}
	std::ifstream infile(file);
	std::string line;
	int count = 0;
	int lines = 0;
	while(std::getline(infile,line)) {
		// std::cout << line << '\n';
		std::vector<int> array = Sel::sellers(line,pat,emax);
		// print_edit(array);
		if(!array.empty()) {
			if(!cflag)
				std::cout << line << '\n';
			count += array.size();
			lines++;
		}
	}
	if(cflag)
		std::cout << "[Lines - " << lines << "] , [Count - "
		<< count << "], Sellers" <<'\n';

	infile.close();
}

void wu_call(char *file, std::string pat, int emax, bool cflag){
    //TODO: Function and sel_aux , that returns the count, {if(prox-atual > 1)count++}
	std::ifstream infile(file);
	std::string line;
	std::string ab;
	for(int i =0; i<128; i++) {
		char a = i;
		ab += a;
	}

	int patSize = pat.length();
    std::bitset<BITSET_SIZE> beginMask = std::bitset<BITSET_SIZE>();
    for (int i = 0;i < patSize;i++) { beginMask.set(i, true); }
	std::map<char, std::bitset<BITSET_SIZE>> C = Wu::make_mask(pat,ab,beginMask);

	int count = 0;
	int lines = 0;
	while(std::getline(infile,line)) {
		// std::cout << line << '\n';
		std::vector<int> array = Wu::wu_manber(patSize,line,C,emax,beginMask);
		if(!array.empty()) {
			if(!cflag)
				std::cout << line << '\n';
			lines++;
			count += array.size();
		}
	}
	if(cflag)
		std::cout << "[Lines - " << lines << "] , [Count - "
		<< count << "], WuManber" <<'\n';
	infile.close();

}
/*Chamada dos algoritmos, incluindo a cflag
   Deve ser a chamada final da função call_pmt*/
void call_alg(std::string alg_name, char* file,
              std::vector<std::string> pat_array, int emax, bool cflag){

	if(alg_name.compare(alg_array[0]) == 0) {
		//TODO: BoyerMoore
		// simple_search(file, pat_array, cflag);
		for(auto i : pat_array)
			boy_call(file,i,cflag);
	}
	else if(alg_name.compare(alg_array[1]) == 0) {
		//TODO: Aho -  ahocorasik
		aho_call(file,pat_array,cflag);
	}
	else if(alg_name.compare(alg_array[2]) == 0) {
		//TODO: sel - Sellers
        for(auto i: pat_array)
            sel_call(file,i,emax,cflag);
	}
	else if(alg_name.compare(alg_array[3]) == 0) {
		//TODO: wu - WuManber
		for(auto i: pat_array)
			wu_call(file,i,emax,cflag);
	}
	else{
		std::cout << "Invalid algorithm name" << '\n';
		usage("./pmt", false);
	}
	// infile.close();
}

/*Controller for call the algorithms*/
/* Cada linha deve ser impressa apenas uma vez, independente
   da quantidade de ocorrencias que ela contenha.*/
void call_pmt(std::string alg_name, char* file,
              std::vector<std::string> pat_array, int emax, bool aflag, bool cflag){
	//TODO: Defaults e Sugestões

	//If no algorithm, pflag is activated...
	if(!aflag){
		if(pat_array.size() > 1)
			alg_name = "aho";
		if(emax != -1){
			alg_name = "wu";
		}
	}
	//If no error is selected., default is 1
	if(emax == -1){
		emax = 1;
	}

	call_alg(alg_name, file, pat_array, emax, cflag);
}

int main(int argc, char *argv[]) {
	int option;
	bool eflag = false;
	bool pflag = false;
	bool aflag = false;
	bool cflag = false;
	int txt_index = 2;
	int emax = -1;
	std::string alg_name = "bm";
	std::vector<std::string> pat_array;

	//check_args(argc, txt_index, argv[0]);

	while((option = getopt_long(argc,argv, "e:p:a:ch",
	                            long_options, NULL)) != -1) {
		switch(option) {
		case 'e':
			eflag = true;
			set_emax(optarg, emax);
			//TODO: Busca aproximada PRIORITY 1
			set_txt_index(2,txt_index);
			break;
		case 'p':
			pflag = true;
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
			break;
		case 'h':
			std::cout << "/* message */" << '\n';
			usage(argv[0],true);
			break;
		default:
			return -1;
		}
	}
	check_file(argv[argc-1], argv[0]);
	check_args(argc, txt_index, argv[0]);
	std::cout << argv[txt_index-1] << '\n';
	set_pat(pat_array, argv[txt_index-1],pflag);

	if(pat_array.size() == 1) {
		//é uma string
		std::cout << "é uma string --" << argv[txt_index-1] << "--"<<'\n';
	}
	else{
		//vetor de strings
		std::cout << "é um vetor de string - " << pat_array[0]<<'\n';
	}

	for (size_t i = txt_index; i < argc; i++) {
		/* code */
		std::cout << RED << argv[i] << ":" << RESET << '\n';
		call_pmt(alg_name, argv[i], pat_array, emax, aflag,cflag);
	}




	return 0;
}
