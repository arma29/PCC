//
// Created by Allan on 16/10/2018.
//

#ifndef PCC_AHOCOHASICK_H
#define PCC_AHOCOHASICK_H

#include <map>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <deque>

namespace aho {
    using G = std::map<std::pair<int, char>, int>;
    using O = std::vector<std::vector<int>>;
    using IntPair = std::pair<int, int>;
}

class AhoCohasick {
private:

    std::tuple<aho::G, int, aho::O> build_goto(std::vector<std::string>&, std::string&);
    std::pair<std::vector<int>, aho::O> build_fail(std::vector<std::string>&, std::string&, aho::G&, int, aho::O&);
    std::tuple<aho::G, std::vector<int>, aho::O> build_fsm(std::vector<std::string>&, std::string&);
    void print_fsm(aho::G&, std::vector<int>&, aho::O&, std::string&);
public:
    std::vector<std::vector<int>> ahocohasick(std::string&, std::vector<std::string>&, std::string&);
};

#endif //PCC_AHOCOHASICK_H
