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
#include "util.h"

/*Necessary for getopt_long, use as argument 4*/
static struct option const long_options[]=
{
	{"help", no_argument, NULL, 'h'},
	{"count", no_argument, NULL, 'c'},
	{"pattern", required_argument, NULL, 'p'},
	{0,0,0,0},     //end of array
};

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}


void call_index(const std::string &txtfile){
	double t = clock();

	//Leitura .txt
	std:: string txt = UTL::get_file_contents(txtfile.c_str());
	int n = txt.length();
	// printf("Build Read time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);

	//Criação dos vetores
	// t = clock();
	std::vector<std::vector<int> > P = SAr::build_P(txt,n);
	// printf("Build P time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);

	// t = clock();
	std::vector<int> SArr = SAr::buildSArr(P,n);
	// printf("Build SArray time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);


	// t = clock();
	std::vector<int> Llcp, Rlcp;
	SAr::lcplr(Llcp, Rlcp, SArr, P, n);
	// printf("Build Llcp & Rlcp time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);
	printf("[LR] Build Array time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);
	// printf("%lf, ", (clock() - t) / CLOCKS_PER_SEC);

	//Criação do index
	/*First insert SArr as str + "$" + str(Llcp) + "$"
	+ str(Rlcp) + "$" + text*/
	std::string finalTxt = UTL::indexCreate(SArr,Llcp,Rlcp, txt, "$");


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
	printf("LZ78 Trie Encode time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);
	// printf("%lf, ",(clock() - t) / CLOCKS_PER_SEC );


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
                 const std::string &filename, bool pflag){

	std::string ab;
	for(int i =0; i<256; i++) {
		char a = i;
		ab += a;
	}

	//Leitura do idx
	std::string compressed = UTL::get_file_contents(filename.c_str());

	double taux = clock();
	std::string finalTxt = LZ78_TRIE::decode(compressed,ab);
	printf("LZ78 Trie Decode time: %lfs \n", (clock() - taux) / CLOCKS_PER_SEC);
	// printf("%lf \n", ((clock() - taux) / CLOCKS_PER_SEC));


	//Retorno do index
	std::vector<int> SArr;
	std::vector<int> Llcp, Rlcp;

	std::string redoStr =
		UTL::indexRecovery(SArr,Llcp,Rlcp, finalTxt,'$');
	// std::cout << redoStr.length() << ", " << pat_array[0].length() << ", ";
	//Search
	double count = 0;

	int total = 0;
	double t2 = clock();
	for(auto i : pat_array) {

		if(i.length() > 0){
			double t = clock();
			// std::cout << "Pat: [" << i  << "]"<< '\n';
			// std::cout << "Txt Len: " << redoStr.length() << " bytes"<< '\n';
			int fst = SAr::search(Llcp, Rlcp, SArr, redoStr,redoStr.length(),i);
			std::cout << "Occurences: " << fst << '\n';
			total += fst;
			// double taux = (clock() - t) / CLOCKS_PER_SEC;
			printf("Search time: %lfs \n", (clock() - t) / CLOCKS_PER_SEC);
			// t2 += (clock() - t)/CLOCKS_PER_SEC;
			count++;
		}
	}
	if(pflag){
		printf("Avg time: %lfs in %d lines -> %d Total Occ\n",
			(clock() - t2)/CLOCKS_PER_SEC, (int)count,total);
		// printf("[Pat Len: %d] Avg time: %lfs\n", int(pat_array[0].length()), (clock() - t2)/CLOCKS_PER_SEC);
		//printf("%lf \n", (clock() - t2)/CLOCKS_PER_SEC);
	}

}

int main(int argc, char *argv[]) {
	int option;
	bool pflag = false;
	bool cflag = false;
	int txt_index = 2;
	std::vector<std::string> pat_array;

	if(argc < 2)
		UTL::usage(argv[0],false);

	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		UTL::usage(argv[0],true);


	else if(strcmp(argv[1], "index") == 0 ) {
		if(argc != 3) {
			UTL::usage(argv[0], true);
		}
		// std::cout << filesize(argv[argc-1]) << ", " <<
		// argv[argc-1] << ", ";
		call_index(argv[2]);
	}
	else if(strcmp(argv[1], "search") == 0) {

		while((option = getopt_long(argc,argv, "p:ch",
									long_options, NULL)) != -1) {
			switch(option) {
			case 'p':
				pflag = true;

				UTL::build_string_array(optarg, pat_array);
				UTL::set_txt_index(1,txt_index);
				break;
			case 'c':
				cflag = true;
				UTL::set_txt_index(1,txt_index);
				break;
			case 'h':
				UTL::usage(argv[0],true);
				break;
			default:
				UTL::usage(argv[0],false);
				return -1;
			}
		}

		UTL::set_pat(pat_array, argv[txt_index],pflag);
		// std::cout << filesize(argv[argc-1]) << ", " <<
		// argv[argc-1] << ", ";

		call_search(pat_array, argv[argc-1], pflag);
	}
	else{
		UTL::usage(argv[0],false);
	}


	return 0;
}
