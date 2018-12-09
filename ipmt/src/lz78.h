#ifndef LZ_78
#define LZ_78

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <iostream>

namespace LZ78 {
    namespace Dict {
        std::pair<int, int> index(std::string&,
                                  std::map<std::string, int>&);
        std::string find(int,
                         std::vector<std::string>&);
        void  add(std::string&,
                  std::vector<std::string>&,
                  std::map<std::string, int>&);
        std::string to_str(std::vector<std::string>&);
    };

    std::string int_encode(int, std::string&);
    int int_decode(std::string&, std::string&);
    std::string gprime(std::string&, std::string&);
    std::string cw_encode(std::string&, std::string&);
    std::string encode(std::string&, std::string&);
    std::string decode(std::string&, std::string&);
};

#endif