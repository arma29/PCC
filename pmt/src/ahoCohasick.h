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

namespace Aho {
    using G = std::map<std::pair<int, char>, int>;
    using O = std::vector<std::vector<int>>;
    using IntPair = std::pair<int, int>;

    std::tuple<G, int, O> build_goto(std::vector<std::string>&, std::string&);
    std::pair<std::vector<int>, O> build_fail(std::vector<std::string>&, std::string&, G&, int, O&);
    std::tuple<G, std::vector<int>, O> build_fsm(std::vector<std::string>&, std::string&);
    void print_fsm(G&, std::vector<int>&, O&, std::string&);
    std::vector<std::vector<int>> ahocohasick(std::string&, std::vector<std::string>&, std::string&);
}

#endif //PCC_AHOCOHASICK_H