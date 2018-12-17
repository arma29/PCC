#ifndef LZ_78_trie
#define LZ_78_trie

#include <vector>
#include <string>
#include "stringref.h"
#include <map>
#include <tuple>
#include <utility>
#include <iostream>
#include <memory>

namespace LZ78_TRIE {
    namespace Dict {
        struct node {
            int id = 0;
            std::vector<string_ref> names;
            std::vector<std::shared_ptr<node>> refs;
        };
        std::pair<std::shared_ptr<LZ78_TRIE::Dict::node>, int> search(string_ref& txt,
                                                                 int ini,
                                                                 std::shared_ptr<LZ78_TRIE::Dict::node>);
        std::pair<int, int> index(string_ref&,
                                  std::shared_ptr<node>);
        string_ref find(int,
                         std::vector<string_ref>&);
        void  add(string_ref&,
                  std::vector<string_ref>&,
                  std::shared_ptr<node>);
        string_ref to_str(std::vector<string_ref>&);
    };

    string_ref int_encode(int, string_ref&);
    int int_decode(string_ref&, string_ref&);
    string_ref gprime(string_ref&, string_ref&);
    string_ref cw_encode(string_ref&, string_ref&);
    string_ref encode(string_ref&, string_ref&);
    string_ref decode(string_ref&, string_ref&);
};

#endif