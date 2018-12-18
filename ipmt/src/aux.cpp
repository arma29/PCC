#include "aux.h"
//Teste
void Faux::usage(std::string const& name, bool status)
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
Example: "<< name << " search -p dic.txt text.idx\n" << '\n';


		std::cout <<"\
Option Interpretation:\n\
    -p, --pattern=PFile     Make the search of all patterns in patternfile\n\
    -c, --count             Print the total count of matches (Default)\n\n" << '\n';

		std::cout << "\
Miscellaneous:\n\
    -h, --help              Display this help text and exit\n\n" << '\n';

		std::cout << "\
Relate the bugs to: arma@cin.ufpe.br // mapa@cin.ufpe.br\n\
IPMT Page: <http://www.github.com/arma29/PCC>\n" << '\n';
	}

	exit(EXIT_SUCCESS);
}

std::string Faux::get_file_contents(const char * filename)
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
void Faux::build_string_array(const char *file,
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
void Faux::set_txt_index(int opt, int& index){
	index = index + opt;
}

/*Caso seja uma string, anexo a um vetor de string (único)*/
void Faux::set_pat(std::vector<std::string> &pat_array,
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

int returnIndex(int index, const std::string &str,
	std::vector<int> &backvec, const char SPECC){
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

std::string Faux::indexCreate(const std::vector<int> &SArr,
                const std::vector<int> &Llcp,
                const std::vector<int> &Rlcp,
				const std::string &txt,
				const std::string &SPEC){


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

	return finalTxt;

}

std::string Faux::indexRecovery(std::vector<int> &SArr,
                std::vector<int> &Llcp,
                std::vector<int> &Rlcp,
				const std::string &finalTxt,
				const char SPEC){

	int fst = returnIndex(0, finalTxt, SArr, '$');
	int snd = returnIndex(fst+1, finalTxt, Llcp, '$');
	int thd = returnIndex(snd+1, finalTxt, Rlcp, '$');
	std::string redoStr = finalTxt.substr(thd+1);

	return redoStr;
}
