//
// Created by Allan on 17/10/2018.
//

#ifndef PCC_BOYERMOORE_H
#define PCC_BOYERMOORE_H

#include <map>
#include <deque>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

namespace Bm {
    std::map<char, int> bad_char(std::string&, std::string&);
    std::vector<int> init_border(std::string&);
    std::vector<int> good_suffix(std::string&);
    std::vector<int> boyer_moore(std::string&, std::string&, std::string&);
};

#endif //PCC_BOYERMOORE_H
