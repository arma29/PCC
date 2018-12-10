#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iterator>


#include "sarray.h"
#include "lz78.h"
#include "lz78_trie.h"

/*Necessary for getopt_long, use as argument 4*/
static struct option const long_options[]=
{
	{"help", no_argument, NULL, 'h'},
	{"count", no_argument, NULL, 'c'},
	{"pattern", required_argument, NULL, 'p'},
	{0,0,0,0},     //end of array
};

void usage(std::string const& name, bool status)
{
	// Status False
	if(!status) {
		std::cout << "Usage: " << name
		          << " [MODE] [OPTION] [PATTERN] [FILE]" <<'\n';
		std::cout << "Try '" << name
		          << " --help' for more information"<<'\n';
		exit(EXIT_FAILURE);
	}
	// Status True
	else{
		std::cout << "Usage (Index Mode): " << name
		          << " index TEXTFILE" <<'\n';
		std::cout << "Generates an .idx file for the .txt entry" << '\n';
		std::cout << "\
Example: "<< name << " index text.txt\n" << '\n';

		std::cout << "Usage (Search Mode): " << name
		          << " search [OPTION] PATTERN INDEXFILE" <<'\n';
		std::cout << "Generates an .txt for the .idx file and Search for PATTERN" << '\n';
		std::cout << "\
Example: "<< name << " search -c -p 'dic.txt' text.idx\n" << '\n';


		std::cout <<"\
Option Interpretation:\n\
    -p, --pattern=PFile     Make the search of all patterns in patternfile\n\
    -c, --count             Print the total count of matches\n\n" << '\n';

		std::cout << "\
Miscellaneous:\n\
    -h, --help              Display this help text and exit\n\n" << '\n';

		std::cout << "\
Relate the bugs to: arma@cin.ufpe.br // mapa@cin.ufpe.br\n\
IPMT Page: <http://www.github.com/arma29/PCC>\n" << '\n';
	}

	exit(EXIT_SUCCESS);
}

/**/
char *read(std:: string filename) {
	FILE *file = fopen(filename.c_str(), "rb");
	if (file == NULL) {
		printf("Failed to open file: %s.\n", filename.c_str());
		exit(0);
	} else {
		fseek(file, 0, SEEK_END);
		int size = (int)ftell(file);
		rewind(file);

		char *data = new char[size + 1];
		data[size] = '\0';
		size_t sizeRead = fread(data, 1, size, file);
		if(sizeRead !=  size) {
			std::cout << "unable to read" << '\n';
			exit(0);
		}
		fclose(file);
		return data;
	}
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

	if(infile.is_open()) {
		std::cout << "No file" << '\n';
		usage("./pmt", false);
	}
}

std::string vecString(std::vector<int> vec){
	std::ostringstream oss;

	if (!vec.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec.begin(), vec.end()-1,
		          std::ostream_iterator<int>(oss, ","));

		// Now add the last element with no delimiter
		oss << vec.back();
	}

	return oss.str();
}

void stringVec(std:: string str, std::vector<int> &backvec){

	for(auto i : str) {
		if(i != ',') {
			backvec.push_back((int)(i-48));
			// std::cout << i << '|';
		}
	}
}

std::string SPEC = "$";
char SPECC ='$';

int returnIndex(int index, std::string str, std::vector<int> &backvec){
	std:: string sub;
	for(int i = index; i < str.length(); i++) {
		if(str[i] == SPECC) {
			sub = str.substr(index, i-index);
			// std::cout << "sub: " << sub << '\n';
			stringVec(sub,backvec);
			return i;
		}
	}

}

void call_index(std::string txtfile){
	char *txt = read(txtfile);
	int n = (int)strlen(txt);

	double t = clock();
	std::vector<std::vector<int> > P = SAr::build_P(txt,n);
	std::vector<int> SArr = SAr::buildSArr(P,n);

	std::vector<int> Llcp, Rlcp;
	SAr::lcplr(Llcp, Rlcp, SArr, P, n);
	printf("Build SArray time: %lfs", (clock() - t) / CLOCKS_PER_SEC);
	std::cout << '\n';

	std::string strSArr = vecString(SArr);
	std::string strLlcp = vecString(Llcp);
	std::string strRlcp = vecString(Rlcp);

	std::string finalTxt;
	finalTxt.append(strSArr);
	finalTxt.append(SPEC);
	finalTxt.append(strLlcp);
	finalTxt.append(SPEC);
	finalTxt.append(strRlcp);
	finalTxt.append(SPEC);
	finalTxt.append(txt);
	// finalTxt.append("$");
	// std::cout << "Final txt tem "<< finalTxt.length() << '\n';
	// std::cout << "Final: "<< finalTxt << '\n';

	std::string idx_filename = txtfile.substr(0, txtfile.size() - 4) + ".idx";
	// std::string idx_filename = "ban.idx";
	FILE *idxfile = fopen(idx_filename.c_str(), "wb");
	if (idxfile == NULL) {
		printf("Couldn't create file: %s.", idx_filename.c_str());
		exit(0);
	}

	std::string ab;
	for(int i =0; i<256; i++) {
		char a = i;
		ab += a;
	}
	// std::string ab = "abn0123456789,$\0";

	// std::string compressed = LZ78::encode(finalTxt, ab);
	t = clock();
	std::string compressed = LZ78_TRIE::encode(finalTxt, ab);
	printf("LZ78 Trie Encode time: %lfs", (clock() - t) / CLOCKS_PER_SEC);
	std::cout << '\n';

	// std::cout << compressed << '\n';
	// std::string backstr = LZ78::decode(compressed,ab);
	// std::cout << "decompressed: " << backstr << " len "<< backstr.length()<<'\n';


	fwrite(compressed.c_str(), sizeof(char), compressed.length(), idxfile);
	std::cout << "Index file: " << "'" << idx_filename << "' was created." << '\n';
	fclose(idxfile);
	delete[] txt;
}

/*Search Mode*/
void call_search(std::vector<std::string> pat_array,
                 char *filename){

	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();

	// std::cout << buffer.str() << '\n';

	std::string ab;
	for(int i =0; i<256; i++) {
		char a = i;
		ab += a;
	}

	std::string compressed = buffer.str();
	// std::string finalTxt = LZ78::decode(compressed,ab);
	double taux = clock();
	std::string finalTxt = LZ78_TRIE::decode(compressed,ab);
	printf("LZ78 Trie Decode time: %lfs", (clock() - taux) / CLOCKS_PER_SEC);
	std::cout << '\n';


	// std::cout << finalTxt << '\n';

	std::vector<int> SArr;

	std::vector<int> Llcp, Rlcp;

	int fst = returnIndex(0, finalTxt, SArr);
	int snd = returnIndex(fst+1, finalTxt, Llcp);
	int thd = returnIndex(snd+1, finalTxt, Rlcp);
	// int fou = returnIndex(thd+1, finalTxt);
	std::string redoStr = finalTxt.substr(thd+1);
	// std::cout << "[" << redoStr << "]"<< '\n';

	char *txt = new char[redoStr.length()+1];
	txt[redoStr.length()] = '\0';
	std::strcpy(txt, redoStr.c_str());
	int n = (int)strlen(txt);

	int count = 0;
	double avg = 0;
	for(auto i : pat_array) {
		std::string pat = i;
		// std::cout << "palavra é [" << i << "]" << '\n';
		char *p = new char[pat.size() + 1];
		pat.copy(p, std::string::npos, 0);
		p[pat.size()] = '\0';

		double t = clock();
		std::cout << "Count: " <<
		SAr::search(Llcp, Rlcp, SArr, txt,n,p) << '\n';
		printf("Search time: %lfs", (clock() - t) / CLOCKS_PER_SEC);
		std::cout << '\n';

		count ++;
		avg += (clock() - t);
		delete[] p;
	}
	std::cout << "Avg time: " << avg/count << '\n';
	delete[] txt;
}

int main(int argc, char *argv[]) {
	int option;
	bool pflag = false;
	bool cflag = false;
	int txt_index = 2;
	std::vector<std::string> pat_array;

	if(argc < 2)
		usage(argv[0],false);

	if(strcmp(argv[1], "index") == 0 ) {
		if(argc != 3) {
			usage(argv[0], true);
		}
		call_index(argv[2]);
	}
	else if(strcmp(argv[1], "search") == 0) {

		while((option = getopt_long(argc,argv, "p:ch",
									long_options, NULL)) != -1) {
			switch(option) {
			case 'p':
				pflag = true;

				build_string_array(optarg, pat_array);
				set_txt_index(1,txt_index);
				break;
			case 'c':
				cflag = true;
				set_txt_index(1,txt_index);
				break;
			case 'h':
				usage(argv[0],true);
				break;
			default:
				usage(argv[0],false);
				return -1;
			}
		}

		set_pat(pat_array, argv[txt_index],pflag);
		call_search(pat_array, argv[argc-1]);
	}
	else{
		usage(argv[0],false);
	}

	return 0;
}
