//
// Created by Allan on 19/10/2018.
//

#ifndef PCC_WUMANBER_H
#define PCC_WUMANBER_H

#include "boost/dynamic_bitset.hpp"
#include <iostream>
#include <map>

namespace Wu {
    std::map<char, boost::dynamic_bitset<>> make_mask(std::string&, std::string&);
    std::vector<int> wu_manber(std::string&, std::string&, std::string&, int r);
};

#endif //PCC_WUMANBER_H
