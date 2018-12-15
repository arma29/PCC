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

std::string get_file_contents(const char * filename)
{
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp)
  {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    size_t ret = std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return(contents);
  }
  throw(errno);
}

/*Convert the input file in array of strings*/
void build_string_array(const char *file,
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
	//std::cout << "[" << str_array[0] << "]" << '\n';
	infile.close();
}

/*Function to set the index of first wildcard argument or textfile*/
void set_txt_index(int opt, int& index){
	index = index + opt;
}

/*Caso seja uma string, anexo a um vetor de string (único)*/
void set_pat(std::vector<std::string> &pat_array,
             const std::string &str,bool pflag){

	if(!pflag)
		pat_array.push_back(str);

	std::ifstream infile(pat_array[0]);

	if(infile.is_open()) {
		std::cout << "No file" << '\n';
		usage("./pmt", false);
	}
}


std::string vecString(const std::vector<int> &vec){
	std::ostringstream oss;

	if (!vec.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec.begin(), vec.end(),
		          std::ostream_iterator<int>(oss, ","));

		// Now add the last element with no delimiter
		oss << vec.back();
	}

	return oss.str();
}

void stringVec(const std:: string &str, std::vector<int> &backvec){
	int count = 0;
	int total = 0;
	for(auto i : str) {
		count++;
		total++;
		// std::cout << i << "-" << '\n';
		if(i == ',') {
			std::string aux = str.substr(total-count,count-1);
			int add = 0;
			for(auto j: aux){
				add += (int)(j - 48)*pow(10,count-2);
				count--;
			}
			backvec.push_back(add);
			// std::cout << i << '|';
			count = 0;
		}
	}
}

std::string SPEC = "$";
char SPECC ='$';

int returnIndex(int index, const std::string &str, std::vector<int> &backvec){
	std:: string sub;
	for(int i = index; i < str.length(); i++) {
		if(str[i] == SPECC) {
			sub = str.substr(index, i-index);
			// std::cout << "sub: " << sub << '\n';
			stringVec(sub,backvec);
			// std::cout << "Back: " << backvec[0] << '\n';
			return i;
		}
	}

}

void call_index(const std::string &txtfile){
	double t = clock();

	//Leitura .txt
	std:: string txt = get_file_contents(txtfile.c_str());
	int n = txt.length();
	printf("Build Read time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);

	//Criação dos vetores
	t = clock();
	std::vector<std::vector<int> > P = SAr::build_P(txt,n);
	printf("Build P time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);

	t = clock();
	std::vector<int> SArr = SAr::buildSArr(P,n);
	printf("Build SArray time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);


	t = clock();
	std::vector<int> Llcp, Rlcp;
	SAr::lcplr(Llcp, Rlcp, SArr, P, n);
	printf("Build Llcp & Rlcp time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);


	//Criação do index
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

	std::string ab;
	for(int i =0; i<256; i++) {
		char a = i;
		ab += a;
	}
	
	//Compress
	t = clock();
	std::string compressed = LZ78_TRIE::encode(finalTxt, ab);
	printf("LZ78 Trie Encode time: %lfs", (clock() - t) / CLOCKS_PER_SEC);
	std::cout << '\n';

	//Write .idx file
	std::string idx_filename = txtfile.substr(0, txtfile.size() - 4) + ".idx";
	FILE *idxfile = fopen(idx_filename.c_str(), "wb");
	if (idxfile == NULL) {
		printf("Couldn't create file: %s.", idx_filename.c_str());
		exit(0);
	}

	fwrite(compressed.c_str(), sizeof(char), compressed.length(), idxfile);
	std::cout << "Index file: " << "'" << idx_filename << "' was created." << '\n';
	fclose(idxfile);

}

void printV(std::vector<int> v){
	std::cout << "[";
	for (size_t i = 0; i < v.size(); i++) {
		/* code */
		if(i != v.size()-1)
			std::cout << v[i] << ",";
		else
			std::cout << v[i];
	}
	std::cout << "]"<< '\n';
}

/*Search Mode*/
void call_search(const std::vector<std::string> &pat_array,
                 const std::string &filename){

	std::string ab;
	for(int i =0; i<256; i++) {
		char a = i;
		ab += a;
	}

	//Leitura do idx
	std::string compressed = get_file_contents(filename.c_str());

	double taux = clock();
	std::string finalTxt = LZ78_TRIE::decode(compressed,ab);
	printf("LZ78 Trie Decode time: %lfs \n", (clock() - taux) / CLOCKS_PER_SEC);


	//Retorno do index
	std::vector<int> SArr;
	std::vector<int> Llcp, Rlcp;

	int fst = returnIndex(0, finalTxt, SArr);
	int snd = returnIndex(fst+1, finalTxt, Llcp);
	int thd = returnIndex(snd+1, finalTxt, Rlcp);
	std::string redoStr = finalTxt.substr(thd+1);

	//Search
	double count = 0;
	double t2 = clock();
	for(auto i : pat_array) {

		if(i.length() > 0){
			double t = clock();
			std::cout << "Pat: [" << i  << "]"<< '\n';
			std::cout << "Txt Len: " << redoStr.length() << " bytes"<< '\n';
			std::cout << "Occurences: " <<
			SAr::search(Llcp, Rlcp, SArr, redoStr,redoStr.length(),i) << '\n';
			printf("Search time: %lfs \n\n", (clock() - t) / CLOCKS_PER_SEC);

			count++;
		}
	}
	printf("Avg time: %lfs in %d counts\n", ((clock()-t2)/CLOCKS_PER_SEC)/count , (int)count);
}

int main(int argc, char *argv[]) {
	int option;
	bool pflag = false;
	bool cflag = false;
	int txt_index = 2;
	std::vector<std::string> pat_array;

	if(argc < 2)
		usage(argv[0],false);

	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		usage(argv[0],true);

	else if(strcmp(argv[1], "index") == 0 ) {
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
