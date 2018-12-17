#ifndef S_ARRAY
#define S_ARRAY

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
    std::vector<std::vector<int> > build_P(const std:: string &txt,int n);
    std::vector<int> buildSArr(const std::vector<std::vector<int> > &P,int n);
    void lcplr(std::vector<int> &Llcp,
               std::vector<int> &Rlcp,
               const std::vector<int> &SArr,
               const std::vector<std::vector<int> > &P,
               int n);
    int search(const std::vector<int> &Llcp,
               const std::vector<int> &Rlcp,
               const std::vector<int> &SArr,
               const std::string &txt, int n,
               const std::string &pat);
}

#endif
