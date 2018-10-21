//
// Created by Allan on 21/10/2018.
//

#ifndef PCC_SHIFTOR_H
#define PCC_SHIFTOR_H

#include <bitsring>
#include "config.h"

namespace Sho {
    std::map<char, std::bitset<BITSET_SIZE>> make_mask(std::string&, std::string&, std::bitset<BITSET_SIZE>&);
};

#endif //PCC_SHIFTOR_H
