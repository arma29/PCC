//
// Created by Allan on 19/10/2018.
//

#include "wumanber.h"

std::map<char, boost::dynamic_bitset<>> Wu::make_mask(std::string &pat, std::string &ab) {
    int l = pat.length(); int n = ab.length();
    std::map<char, boost::dynamic_bitset<>> mask;
    boost::dynamic_bitset<> lastOne = boost::dynamic_bitset<>(l).set(l-1,1);
    boost::dynamic_bitset<> slide = boost::dynamic_bitset<>(lastOne).flip();
    for (int i = 0;i < n;i++) { mask[ab[i]] = boost::dynamic_bitset<>(l).set(); }
    for (int j = 0;j < l;j++) {
        mask[pat[j]] = mask[pat[j]] & slide;
        slide = (slide >> 1) | lastOne;
    }
    return mask;
}

std::vector<int> Wu::wu_manber(std::string &txt,
                               std::string &pat,
                               std::string &ab,
                               int r) {
    int n = txt.length();
    int m = pat.length();
    std::map<char, boost::dynamic_bitset<>> C = make_mask(pat, ab);
    std::vector<int> occ;
    std::vector<boost::dynamic_bitset<>> s = std::vector<boost::dynamic_bitset<>>(r + 1, boost::dynamic_bitset<>(m).flip());
    boost::dynamic_bitset<> old;
    boost::dynamic_bitset<> old2;
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

int main() {
    std::string ab = "abcdefghijklmnopqrstuvwxyz1234567890 \n\r\0\t";
    std::string pat = "like";
    std::string txt = "love";
    std::vector<int> occ = Wu::wu_manber(txt, pat, ab, 2);

    std::cout << "[";
    for (int i = 0;i < occ.size();i++) {
        std::cout << occ[i] << ", ";
    }
    std::cout << "]" << std::endl;
    return 0;
}