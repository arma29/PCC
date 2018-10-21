//
// Created by Allan on 21/10/2018.
//

#include "shiftOr.h"

std::map<char, std::bitset<BITSET_SIZE>> Sho::make_mask(std::string &pat, std::string &ab,
                                                       std::bitset<BITSET_SIZE> &beginMask) {
    int l = pat.length(); int n = ab.length();
    std::map<char, std::bitset<BITSET_SIZE>> mask;

    std::bitset<BITSET_SIZE> lastOne = std::bitset<BITSET_SIZE>().set(l-1,true) & beginMask;
    std::bitset<BITSET_SIZE> slide = std::bitset<BITSET_SIZE>(lastOne).flip() & beginMask;
    for (int i = 0;i < n;i++) { mask[ab[i]] = std::bitset<BITSET_SIZE>().set() & beginMask; }
    for (int j = 0;j < l;j++) {
        mask[pat[j]] = mask[pat[j]] & slide;
        slide = (slide >> 1) | lastOne;
    }
    return mask;
}

std::vector<int> Sho::shift_or(int m,
                               std::string &txt,
                               std::map<char, std::bitset<BITSET_SIZE>> &C,
                               std::bitset<BITSET_SIZE> &beginMask) {
    int n = txt.length();
    std::vector<int> occ;
    std::bitset<BITSET_SIZE> s = std::bitset<BITSET_SIZE>().flip() & beginMask;
    for (int i = 0;i < n;i++) {
        s = (s >> 1) | C[txt[i]];
        if (!s.test(0)) {
            occ.push_back(i - m + 1);
        }
    }
    return occ;
}