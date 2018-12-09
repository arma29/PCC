#ifndef LZ_78_trie
#define LZ_78_trie

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <iostream>
#include <memory>

namespace LZ78 {
    namespace Dict {
        struct node {
            int id = 0;
            std::vector<std::string> names;
            std::vector<std::shared_ptr<node>> refs;
        };
        std::pair<std::shared_ptr<LZ78::Dict::node>, int> search(std::string& txt,
                                                                 int ini,
                                                                 std::shared_ptr<LZ78::Dict::node>);
        std::pair<int, int> index(std::string&,
                                  std::shared_ptr<node>);
        std::string find(int,
                         std::vector<std::string>&);
        void  add(std::string&,
                  std::vector<std::string>&,
                  std::shared_ptr<node>);
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