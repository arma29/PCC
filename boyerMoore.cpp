//
// Created by Allan on 17/10/2018.
//

#include "boyerMoore.h"

std::map<char, int> BoyerMoore::bad_char(std::string &pat, std::string &ab) {
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

bool BoyerMoore::ends_with(std::string &a, std::string &b) {
    int lenA = a.length();
    int lenB = b.length();

    if (lenA >= lenB) {
        return (0 == a.compare (lenA - lenB, lenB, b));
    } else {
        return false;
    }
}

bool BoyerMoore::sim(std::string &a, std::string &b) {
    return ends_with(a,b) || ends_with(b,a);
}

int BoyerMoore::border_bf(std::string &s) {
    int n = s.length();
    if (n == 0) {
        return -1;
    }
    std::string first = "";
    std::string last = "";
    for (int i = n-1; i > 0; i--) {
        first = s.substr(0, i);
        last = s.substr(n - i, n);
        if (!first.compare(last)) {
            return i;
        }
    }
    return 0;
}

std::vector<int> BoyerMoore::init_border(std::string &pat) {
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

std::vector<int> BoyerMoore::good_suffix_bf(std::string &pat) {
    int m = pat.length();
    std::vector<int> S(m+1, m);

    std::string first = "";
    std::string last = "";
    for (int j = 0; j < m; j++) {
        for (int k = m - 1; k > 0; k--) {
            first = pat.substr(j+1, m);
            last = pat.substr(0, k);
            if (sim(first, last)) {
                S[j] = m - k;
                break;
            }
        }
    }
    S[m] = m - border_bf(pat);
    return S;
}

std::vector<int> BoyerMoore::good_suffix(std::string & pat) {
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

std::vector<int> BoyerMoore::boyerMoore(std::string &txt, std::string &pat, std::string &ab) {
    int n = txt.length();
    int m = pat.length();
    int l = ab.length();

    std::map<char, int> C = bad_char(pat, ab);
    std::vector<int> S = good_suffix(pat);

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
//    BoyerMoore b;
//    std::string txt = "Ah! the love! love, love, love... I love it!";
//    std::string pat = "love";
//    std::string ab = "";
//
//    for (int i = 0;i < 256;i++) {
//        ab += (char) i;
//    }
//
//    for (auto i: b.boyerMoore(txt, pat, ab)) {
//        std::cout << i << ", ";
//    }
//    std::cout << std::endl;
//    return 0;
//}