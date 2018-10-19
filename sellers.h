//
// Created by Arnaldo Morais
//

#ifndef PCC_SELLERS_H
#define PCC_SELLERS_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class Sellers {
private:
int phi(char a, char b);
std::vector<std::vector<int> > mtz(int n, int m);
public:
std::vector<int> sellers(std::string txt,
                         std::string pat, int emax);
};

#endif
