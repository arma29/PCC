#include "lz78_trie.h"

std::pair<std::shared_ptr<LZ78_TRIE::Dict::node>, int> LZ78_TRIE::Dict::search(string_ref& txt,
                                                                     int ini,
                                                                     std::shared_ptr<LZ78_TRIE::Dict::node> curr) {
    string_ref name;
    string_ref prefix;
    bool miss = false;
    while (!miss) {
        miss = true;
        for (int i = 0; i < curr->names.size();i++) {
            name = curr->names.at(i);
            prefix = string_ref(txt, ini, name.length());
            if (name == prefix) {
                ini = ini + prefix.length();
                curr = curr->refs[i];
                miss = false;
                break;
            }
        }
    }
    return std::make_pair(curr, ini);
}

std::pair<int, int> LZ78_TRIE::Dict::index(string_ref& txt,
                                      std::shared_ptr<LZ78_TRIE::Dict::node> root) {
    std::shared_ptr<LZ78_TRIE::Dict::node> found;
    int end;
    std::tie(found, end) = LZ78_TRIE::Dict::search(txt, 0, root);
    return std::make_pair(found->id, end); 
}

string_ref LZ78_TRIE::Dict::find(int i,
                             std::vector<string_ref>& data_inv) {
    if (i > data_inv.size()) {
        string_ref ret("");
        return ret;
    }
    return data_inv[i];
}

void LZ78_TRIE::Dict::add(string_ref& w,
                    std::vector<string_ref>& data_inv,
                    std::shared_ptr<LZ78_TRIE::Dict::node> root) {
    std::shared_ptr<LZ78_TRIE::Dict::node> found;
    int end;
    std::tie(found, end) = LZ78_TRIE::Dict::search(w, 0, root);

    std::shared_ptr<LZ78_TRIE::Dict::node> newNode(new LZ78_TRIE::Dict::node);
    newNode->id = data_inv.size();
    found->names.push_back(string_ref(w, end, w.length()));
    found->refs.push_back(newNode);
    data_inv.push_back(w);
}

string_ref LZ78_TRIE::int_encode(int x, string_ref& ab) {
    if (x == 0) {
        string_ref ret(ab, 0, 1);
        return ret;
    }

    int base = ab.length();
    string_ref code("");
    int bit;

    while (x) {
        bit = x % base;
        code = ab[bit] + code;
        x /= base;
    }

    return code;
}

int LZ78_TRIE::int_decode(string_ref& x, string_ref & ab) {
    int base = ab.length();
    int power = 1;
    int val = 0;
    int found;

    for (int i = x.length() - 1; i >= 0; i--) {
        found = ab.find(x[i]);
        
        if (found == std::string::npos) {
            return -1;
        }

        val = val + found*power;
        power *= base;
    }
    return val;
}

string_ref LZ78_TRIE::gprime(string_ref& y, string_ref& ab) {
    if (y.length() <= 1) {
        return y;
    } else {
        string_ref encoded = LZ78_TRIE::int_encode(y.length() - 2, ab);
        return gprime(encoded, ab) + y;
    }
}

string_ref LZ78_TRIE::cw_encode(string_ref& w, string_ref& ab) {
    string_ref aux = string_ref(ab, 1,1) + w;
    return LZ78_TRIE::gprime(aux, ab) + string_ref(ab, 0,1);
}

string_ref LZ78_TRIE::encode(string_ref& txt, string_ref& ab) {
    string_ref code;
    int n = txt.length();
    int i = 0;
    int j, l;

    string_ref int_encoded;
    string_ref cj;
    string_ref to_add;

    std::vector<string_ref> data_inv;
    std::shared_ptr<LZ78_TRIE::Dict::node> root(new LZ78_TRIE::Dict::node);
    data_inv.push_back("");

    while (i < n) {
        string_ref sufix = txt.substr(i, n);

        std::tie(j, l) = LZ78_TRIE::Dict::index(sufix, root);
        int_encoded = int_encode(j, ab);
        cj = LZ78_TRIE::cw_encode(int_encoded, ab);
        code = code + cj + txt.substr(i+l, 1);
        to_add = txt.substr(i, l+1);
        LZ78_TRIE::Dict::add(to_add, data_inv, root);
        i += l + 1;
    }
    return code;
}

string_ref LZ78_TRIE::decode(string_ref& code, string_ref& ab) {
    std::vector<string_ref> data_inv;
    std::shared_ptr<LZ78_TRIE::Dict::node> root(new LZ78_TRIE::Dict::node);
    data_inv.push_back("");

    int i = 0;
    int n = code.length();
    string_ref txt("");

    string_ref w;
    int l;

    string_ref sufix;
    string_ref dic_entry;
    string_ref c;
    string_ref aux;

    while (i < n) {
        w = string_ref(code, i, 1);
        l = LZ78_TRIE::int_decode(w, ab);
        i += 1;
        while (1) {
            if (string_ref(code, i, 1) == string_ref(ab, 0, 1)) {
                sufix = string_ref(w, 1, w.length() - 1);
                dic_entry = LZ78_TRIE::Dict::find(LZ78_TRIE::int_decode(sufix, ab), data_inv);
                txt += dic_entry;
                i++;
                c = string_ref(code, i, 1);
                txt += c;
                i+=1;
                aux = dic_entry + c;
                LZ78_TRIE::Dict::add(aux, data_inv, root);
                break; 
            }
            w = string_ref(code ,i, l+2);
            i = i + l + 2;
            l = LZ78_TRIE::int_decode(w, ab);
        }
    }
    return txt;
}
