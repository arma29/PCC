//
// Created by Allan on 18/10/2018.
//

#include "ukkonen.h"

int * Ukkonen::tree_find(Ukkonen::Node * root, std::vector<int> &s) {
    Node * cur = root;
    int i = 1;
    int l = s.size();
    int d = 0;

    while (i < l) {
        d = s[i] - s[i - 1];
        if (cur->chd[d+1] != nullptr) {
            cur = cur->chd[d+1];
            i++;
        }
        else {
            break;
        }
    }

    if (i == l) {
        return &(cur->idx);
    }
    else {
        return nullptr;
    }
}

void Ukkonen::tree_add(Ukkonen::Node * root, std::vector<int> &s, int idx) {
    Ukkonen::Node * cur = root;
    Ukkonen::Node * nn;
    int i = 1;
    int l = s.size();
    int d = 0;

    while (i < l) {
        d = s[i] - s[i-1];
        if (cur->chd[d+1] != nullptr) {
            cur = cur->chd[d+1];
            i++;
        }
        else {
            break;
        }
    }
    while (i < l) {
        nn = new Node();
        nn->idx = idx;
        nn->chd = std::vector<Node*>();
        d = s[i] - s[i - 1];
        cur->chd[d+1] = nn;
        cur = nn;
        i++;
    }
}

std::vector<int> Ukkonen::next_column(std::vector<int> &s, std::string &pat, char a, int r) {
    int m = pat.length();
    std::vector<int> t(m+1, 0);

    for (int i = 1;i < m+1;i++) {
        t[i] = std::min(s[i] + 1, std::min(t[i - 1] + 1, std::min(s[i - 1] + (pat[i-1] != a), r + 1)));
    }
    return t;
}

std::tuple<Uk::Delta, Uk::F, int> Ukkonen::build_ukk_fsm(std::string &pat, std::string &ab, int r) {
    int m = pat.length();
    int abS = ab.size();
    int idx_s = 0;
    int * idx_t;

    std::vector<int> s(m + 1);
    for (int i = 0;i < m + 1;i++) {
        s[i] = i;
    }

    std::vector<int> t;
    char a;

    Uk::Queue queue;
    queue.push_back(std::make_pair(s, 0));

    Node * Q = new Node();
    Q->chd = std::vector<Node*>();

    tree_add(Q, s, 0);

    Uk::F f;
    int idx = 0;
    Uk::Delta delta;

    if (s.back() <= r) {
        f.insert(idx);
    }

    while (!queue.empty()) {

        std::tie(s, idx_s) = queue.front();
        queue.pop_front();
        for (int i = 0; i < abS;i++) {

            a = ab[i];
            t = next_column(s, pat, a, r);
            idx_t = tree_find(Q, t);

            if (idx_t == nullptr) {
                idx++;
                idx_t = &idx;
                tree_add(Q, t, idx);
                queue.push_back(std::make_pair(t, *idx_t));
                if (t.back() <= r) {
                    f.insert(idx);
                }
            }

            delta[std::make_pair(idx_s, a)] = *idx_t;

        }
    }
    return std::make_tuple(delta, f, idx);
}

std::vector<int> Ukkonen::ukkonen(std::string &txt, std::string &pat, std::string &ab, int r) {
    Uk::F f;
    Uk::Delta delta;
    int l;

    std::tie(delta, f, l) = build_ukk_fsm(pat, ab, r);

    int s = 0;
    int n = txt.length();
    std::vector<int> occ;

//    if (f.find(s) != f.end()) {
//        occ.push_back(i);
//    }

    for (int i = 0;i < n;i++) {
        s = delta[std::make_pair(s, txt[i])];
        if (f.find(s) != f.end()) {
            occ.push_back(i);
        }
    }
    return occ;
}

int main() {
    Ukkonen u;
    std::string txt = "Ah! the lore! love, love, love... I love it!";
    std::string pat = "love";
    std::string ab = "";

    for (int i = 0;i < 256;i++) {
        ab += (char) i;
    }

    for (auto i: u.ukkonen(txt, pat, ab, 2)) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
    return 0;
}