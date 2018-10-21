#ifndef PCC_SHIFTOR_H
#define PCC_SHIFTOR_H

#include <bitset>
#include <vector>
#include <map>
#include "config.h"

namespace Sho {
    std::map<char, std::bitset<BITSET_SIZE>> make_mask(std::string&, std::string&, std::bitset<BITSET_SIZE>&);
    std::vector<int> shift_or(int, std::string&, std::map<char, std::bitset<BITSET_SIZE>>&,
                                   std::bitset<BITSET_SIZE>&);
};

#endif //PCC_SHIFTOR_H
