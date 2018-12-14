#ifndef PCC_SELLERS_H
#define PCC_SELLERS_H

#include <algorithm>
#include <cstring>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <math.h>

namespace SAr{
    std::vector<std::vector<int> > build_P(std:: string txt,int n);
    std::vector<int> buildSArr(std::vector<std::vector<int> > &P,int n);
    void lcplr(std::vector<int> &Llcp,
               std::vector<int> &Rlcp,
               std::vector<int> &SArr,
               std::vector<std::vector<int> > &P,
               int n);
    int search(std::vector<int> &Llcp,
               std::vector<int> &Rlcp,
               std::vector<int> &SArr,
               char *txt, int n, const char *pat);
}

#endif
