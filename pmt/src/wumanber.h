//
// Created by Allan on 19/10/2018.
//

#ifndef PCC_WUMANBER_H
#define PCC_WUMANBER_H

#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#define BITSET_SIZE 200

namespace Wu {
    std::map<char, std::bitset<BITSET_SIZE>> make_mask(std::string&, std::string&, std::bitset<BITSET_SIZE>&);
    std::vector<int> wu_manber(int, std::string&, std::map<char, std::bitset<BITSET_SIZE>>&, int, std::bitset<BITSET_SIZE>&);
};

#endif //PCC_WUMANBER_H
