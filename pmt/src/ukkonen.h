//
// Created by Allan on 18/10/2018.
//

#ifndef PCC_UKKONEN_H
#define PCC_UKKONEN_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <deque>

namespace Uk {
    using Delta = std::map<std::pair<int, char>, int>;
    using Queue = std::deque<std::pair<std::vector<int>, int>>;
    using F = std::set<int>;
};

class Ukkonen {
private:
public:
    typedef struct Node{
        int idx;
        std::vector<Node*> chd;
    } Node;

    int * tree_find(Node *, std::vector<int>&);
    void tree_add(Node*, std::vector<int>&, int);
    std::vector<int> next_column(std::vector<int>&, std::string&, char, int);
    std::tuple<Uk::Delta, Uk::F, int> build_ukk_fsm(std::string&, std::string&, int);
    std::vector<int> ukkonen(std::string&, std::string&, std::string&, int);
};

#endif //PCC_UKKONEN_H
