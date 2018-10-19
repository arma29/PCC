#include "ahoCohasick.h"

std::tuple<Aho::G , int, Aho::O> Aho::build_goto(std::vector<std::string> &P, std::string &ab) {
    Aho::G g; Aho::O o;
    o.push_back(std::vector<int>());
    int nxt = 0;
    for (int k = 0; k < P.size(); k++) {
        std::string pat = P[k];
        int m = pat.size();
        int cur = 0; int j = 0;
        while (j < m && g.count(std::make_pair(cur, pat[j]))) {
            cur = g[std::make_pair(cur, pat[j])];
            j++;
        }

        while (j < m) {
            nxt++;
            o.push_back(std::vector<int>());
            g[std::make_pair(cur, pat[j])] = nxt;
            cur = nxt;
            j++;
        }
        o[cur].push_back(k);
        pat.clear();
    }

    Aho::IntPair zero_a(0, 0);
    for (int i = 0; i < ab.size(); i++) {
        zero_a.second = ab[i];
        if (g.count(zero_a) == 0) {
            g[zero_a] = 0;
        }
    }
    std::tuple<Aho::G, int, Aho::O> ret(g, nxt+1, o);
    return ret;
}

std::pair<std::vector<int>, Aho::O> Aho::build_fail(std::vector<std::string> &P,
                                                            std::string &ab,
                                                            Aho::G &g,
                                                            int n,
                                                            Aho::O &o) {
    std::deque<int> Q;
    std::vector<int> f(n, 0);

    Aho::IntPair aux(0,0);

    for (int i = 0;i < ab.size(); i++) {
        aux.second = ab[i];
        if (g[aux] != 0) {
            Q.push_back(g[aux]);
            f[g[aux]] = 0;
        }
    }

    while (Q.size() > 0) {
        int cur = Q[0];
        Q.pop_front();
        for (int i = 0;i < ab.size(); i++) {
            aux.first = cur;
            aux.second = ab[i];
            if (g.count(aux) > 0) {
                int nxt = g[aux];
                Q.push_back(nxt);
                int brd = f[cur];

                aux.first = brd;
                aux.second = ab[i];
                while (g.count(aux) == 0) {
                    brd = f[brd];
                    aux.first = brd;
                }
                aux.first = brd;
                f[nxt] = g[aux];
                o[nxt].reserve(o[nxt].size() + o[f[nxt]].size());
                o[nxt].insert(o[nxt].end(), o[f[nxt]].begin(), o[f[nxt]].end());
            }
        }

    }

    return std::pair<std::vector<int>, Aho::O>(f, o);
}

std::tuple<Aho::G, std::vector<int>, Aho::O> Aho::build_fsm(std::vector<std::string> &P,
                                                                    std::string &ab) {
    Aho::G g; int n; Aho::O o; std::vector<int> f;
    std::tie(g, n, o) = build_goto(P, ab);
    std::tie(f, o) = build_fail(P, ab, g, n, o);
//    print_fsm(g, f, o, ab);
    return std::tuple<Aho::G, std::vector<int>, Aho::O>(g, f, o);
}

std::vector<std::vector<int>> Aho::ahocohasick(std::string &txt, std::vector<std::string> &P, std::string &ab) {
    int n = txt.size(); int cur;
    std::vector<int> m(P.size());
    Aho::G g; Aho::O o; std::vector<int> f;
    std::vector<std::vector<int>> occ;

    for (int i = 0;i < P.size();i++) {
        m[i] = P[i].size();
    }

    std::tie(g, f, o) = build_fsm(P, ab);
    cur = 0;

    for (int i = 0;i < P.size();i++) {
        occ.push_back(std::vector<int>());
    }

    for (int i = 0;i < n;i++) {
            char a = txt[i];
            while (!g.count(std::make_pair(cur, a))) {
                cur = f[cur];
            }
            cur = g[std::make_pair(cur, a)];
            for (int p = 0; p < o[cur].size();p++) {
                occ[o[cur][p]].push_back(i - m[o[cur][p]] + 1);
            }
    }

    return occ;
}

void Aho::print_fsm(Aho::G &g, std::vector<int> &f, Aho::O &o, std::string &ab) {
    int nxt = o.size();
    std::cout << "goto" << std::endl;
    Aho::IntPair aux(0,0);
    for (int s = 0;s < nxt;s++){
        std::cout << s << ":" << std::endl;
        for (int i = 0;i < ab.size();i++) {
            aux.first = s;
            aux.second = ab[i];
            if (g.count(aux) > 0) {
                std::cout << ab[i] << "-->" << g[aux] << std::endl;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "occ" << std::endl;
    for (int s = 0; s < nxt;s++) {
        std::cout << s << ":" << std::endl;
        for (int j = 0; j < o[s].size();j++) {
            std::cout << o[s][j] << ", ";
        }
        std::cout << std::endl;
    }
}

//int main() {
//    std::vector<std::string> P = {"alamo"};
//    std::string txt = "alamoo";
//    std::string ab = "abcdefghijklmnopqrstuvwxyz \n\0\r";
//
//    std::vector<std::vector<int>> b = Aho::ahocohasick(txt, P, ab);
//    std::cout << "[";
//    for (int i = 0;i < b.size(); i++) {
//        std::cout << "[";
//        for (int j  = 0;j < b[i].size();j++) {
//            std::cout << b[i][j] << ", ";
//        }
//        std::cout << "], ";
//    }
//    std::cout << "]" << std::endl;
//    return 0;
//}
