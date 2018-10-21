//
// Created by Allan on 19/10/2018.
//

#include "wumanber.h"

std::map<char, std::bitset<BITSET_SIZE>> Wu::make_mask(std::string &pat, std::string &ab,
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

std::vector<int> Wu::wu_manber(int m,
                               std::string &txt,
                               std::map<char, std::bitset<BITSET_SIZE>> &C,
                               int r,
                               std::bitset<BITSET_SIZE> &beginMask) {
    int n = txt.length();
    std::vector<int> occ;
    std::vector<std::bitset<BITSET_SIZE>> s = std::vector<std::bitset<BITSET_SIZE>>(
                                                                        r + 1,
                                                                        std::bitset<BITSET_SIZE>().flip() & beginMask);
    std::bitset<BITSET_SIZE> old;
    std::bitset<BITSET_SIZE> old2;
    for (int i = 0;i < n;i++) {
        old = s[0];
        s[0] = (s[0] >> 1) | C[txt[i]];
        for (int q = 1; q < r+1; q++) {
            old2 = s[q];
            s[q] = ((s[q] >> 1) | C[txt[i]]) & (s[q-1] >> 1) & (old >> 1) & old;
            old = old2;
        }
        if (!s[r].test(0)) {
            occ.push_back(i);
        }
    }
    return occ;
}

//int main() {
//    std::string ab = "abcdefghijklmnopqrstuvwxyz1234567890 \n\r\0\t";
//    std::string pat = "like";
//    std::string txt = "love";
//    std::vector<int> occ = Wu::wu_manber(txt, pat, ab, 2);
//
//    std::cout << "[";
//    for (int i = 0;i < occ.size();i++) {
//        std::cout << occ[i] << ", ";
//    }
//    std::cout << "]" << std::endl;
//    return 0;
//}