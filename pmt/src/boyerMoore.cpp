//
// Created by Allan on 17/10/2018.
//

#include "boyerMoore.h"

std::map<char, int> Bm::bad_char(std::string &pat, std::string &ab) {
    int m = pat.length();
    int l = ab.length();

    std::map<char, int> C;

    for (int a = 0; a < l; a++) {
        C[ab[a]] = -1;
    }

    for (int j = 0; j < m; j++) {
        C[pat[j]] = j;
    }

    return C;
}

std::vector<int> Bm::init_border(std::string &pat) {
    int m = pat.length();
    std::vector<int> nxt(m+1, 0);
    int i = 1;
    int j = 0;

    while (i + j < m) {
        while (i + j < m && pat[i + j] == pat[j]) {
            j += 1;
            nxt[i + j] = j;
        }
        i += std::max(1, j - nxt[j]);
        j = nxt[j];
    }
    return nxt;
}

std::vector<int> Bm::good_suffix(std::string & pat) {
    int m = pat.length();
    std::string patCopy(pat);
    std::reverse(patCopy.begin(), patCopy.end());
    std::vector<int> R = init_border(patCopy);
    std::vector<int> S(m+1, m - R[m]);
    int j;
    for (int l = 1;l < m+1;l++) {
        j = m - 1 - R[l];
        if (S[j] > l - R[l]) {
            S[j] = l - R[l];
        }
    }
    return S;
}

std::vector<int> Bm::boyer_moore(std::string &txt, std::string &pat, int abSize, std::map<char, int> &C, std::vector<int> &S) {
    int n = txt.length();
    int m = pat.length();
    int l = abSize;

    std::vector<int> occ;
    int i = 0;
    int j = 0;
    while (i <= n-m) {
        j = m - 1;
        while (j >= 0 && txt[i + j] == pat[j]) {
            j--;
        }

        if (j == -1) {
            occ.push_back(i);
            i += S.back();
        }
        else {
            i += std::max(S[j], j - C[txt[i+j]]);
        }
    }
    return occ;
}

//int main() {
//    std::string txt = "Ah! the love! love, love, love... I love it!";
//    std::string pat = "love";
//    std::string ab = "";
//
//    for (int i = 0;i < 256;i++) {
//        ab += (char) i;
//    }
//
//    for (auto i: Bm::boyer_moore(txt, pat, ab)) {
//        std::cout << i << ", ";
//    }
//    std::cout << std::endl;
//    return 0;
//}