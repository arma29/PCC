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
		if(sizeRead !=	size){
			std::cout << "unable to read" << '\n';
			exit(0);
		}
		fclose(file);
		return data;
	}
}


int main(int argc, char *argv[]) {
	int option;
	bool pflag = false;
	bool cflag = false;
	int txt_index = 2;
	std::vector<std::string> pat_array;

	while((option = getopt_long(argc,argv, "p:ch",
	                            long_options, NULL)) != -1) {
		switch(option) {
		case 'p':
			pflag = true;
			// build_string_array(optarg, pat_array);
			// set_txt_index(1,txt_index);
			break;
		case 'c':
			cflag = true;
			// set_txt_index(1,txt_index);
            usage(argv[0],true);
			break;
		case 'h':
			usage(argv[0],false);
			break;
		default:
			return -1;
		}
	}

	// check_file(argv[argc-1], argv[0]);
	// check_args(argc, txt_index, argv[0]);
	// set_pat(pat_array, argv[txt_index-1],pflag);
    //
	// for (size_t i = txt_index; i < argc; i++) {
	// 	/* code */
	// 	std::cout << argv[i] << ":" << '\n';
	// 	call_pmt(alg_name, argv[i], pat_array, emax, aflag,cflag);
	// }




	return 0;
}
