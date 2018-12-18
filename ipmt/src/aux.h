#ifndef AUX_F
#define AUX_F
//Teste

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
#include <math.h>

namespace Faux{
    void usage(std::string const& name, bool status);
    std::string get_file_contents(const char * filename);
    void build_string_array(const char *file,
                            std::vector<std::string> &str_array);
    void set_txt_index(int opt, int& index);
    void set_pat(std::vector<std::string> &pat_array,
                 const std::string &str,bool pflag);
    std::string indexCreate(const std::vector<int> &SArr,
                            const std::vector<int> &Llcp,
                            const std::vector<int> &Rlcp,
                            const std::string &txt,
                            const std::string &SPEC);
    std::string indexRecovery(std::vector<int> &SArr,
                              std::vector<int> &Llcp,
                              std::vector<int> &Rlcp,
                              const std::string &finalTxt,
                              const char SPEC);
}

#endif
