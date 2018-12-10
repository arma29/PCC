#include "lz78.h"

std::pair<int, int> LZ78::Dict::index(std::string& txt,
                                      std::map<std::string, int>& data) {
    int n = txt.length();
    std::string aux;

    if (n == 0 || data.size() == 0) {
        return std::make_pair(-1, -1);
    }

    for (int i = n - 1; i > -1; i--) {
        aux = txt.substr(0, i);
        if (data.find(aux) != data.end()) {
            return std::make_pair(data[aux], i);
        }
    }
    return std::make_pair(-1, -1);
}

std::string LZ78::Dict::find(int i,
                             std::vector<std::string>& data_inv) {
    if (i > data_inv.size()) {
        return std::string("");
    }
    return data_inv[i];
}

void LZ78::Dict::add(std::string& w,
                    std::vector<std::string>& data_inv,
                    std::map<std::string, int>& data) {
    data[w] = data.size();
    data_inv.push_back(w);
}

std::string LZ78::Dict::to_str(std::vector<std::string>& data_inv) {
    std::string output = "";
    for (int i = 0; i < data_inv.size(); i++) {
        output = output + std::to_string(i) + std::string(":");
        output = output + data_inv[i] + std::string("\n");
    }
    return output;
}

std::string LZ78::int_encode(int x, std::string& ab) {
    if (x == 0) {
        return ab.substr(0, 1);
    }

    int base = ab.length();
    std::string code = "";
    int bit;

    while (x) {
        bit = x % base;
        code = ab[bit] + code;
        x /= base;
    }

    return code;
}

int LZ78::int_decode(std::string& x, std::string & ab) {
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

std::string LZ78::gprime(std::string& y, std::string& ab) {
    // std::cout << "gprime encoding " << y << std::endl;
    if (y.length() <= 1) {
        return y;
    } else {
        std::string encoded = LZ78::int_encode(y.length() - 2, ab);
        return gprime(encoded, ab) + y;
    }
}

std::string LZ78::cw_encode(std::string& w, std::string& ab) {
    // std::cout << "g encoding " << w << std::endl;
    std::string aux = ab.substr(1,1) + w;
    return LZ78::gprime(aux, ab) + ab.substr(0,1);
}

std::string LZ78::encode(std::string& txt, std::string& ab) {
    std::string code;
    int n = txt.length();
    int i = 0;
    int j, l;

    std::string int_encoded;
    std::string cj;
    std::string to_add;

    std::vector<std::string> data_inv;
    std::map<std::string, int> data;
    data[""] = 0;
    data_inv.push_back("");

    while (i < n) {
        std::string sufix = txt.substr(i, n);
        // std::cout << txt.substr(0, i) << "|" << sufix << std::endl;
        // std::cout << LZ78::Dict::to_str(data_inv) << std::endl;
        // std::cout << std::endl;

        std::tie(j, l) = LZ78::Dict::index(sufix, data);
        // std::cout << "found" << LZ78::Dict::find(j, data_inv) << std::endl;
        int_encoded = int_encode(j, ab);
        cj = LZ78::cw_encode(int_encoded, ab);
        // std::cout << "g(" << j << ")=" << cj << std::endl;
        code = code + cj + txt.substr(i+l, 1);
        // std::cout << "code=" << code << std::endl;
        to_add = txt.substr(i, l+1);
        LZ78::Dict::add(to_add, data_inv, data);
        i += l + 1;
    }
    return code;
}

    std::string LZ78::decode(std::string& code, std::string& ab) {
    std::vector<std::string> data_inv;
    std::map<std::string, int> data;
    data[""] = 0;
    data_inv.push_back("");

    int i = 0;
    int n = code.length();
    std::string txt = "";

    std::string w;
    int l;

    std::string sufix;
    std::string dic_entry;
    std::string c;
    std::string aux;

    while (i < n) {
        w = code.substr(i, 1);
        l = LZ78::int_decode(w, ab);
        i += 1;
        while (1) {
            if (code.substr(i, 1) == ab.substr(0, 1)) {
                sufix = w.substr(1, w.length() - 1);
                dic_entry = LZ78::Dict::find(LZ78::int_decode(sufix, ab), data_inv);
                // std::cout << "sufix [" << sufix << "]" << std::endl;
                // std::cout << "dec [" << LZ78::int_decode(sufix, ab) << "]" << std::endl;
                // std::cout << "dic entry [" << dic_entry << "]" << std::endl;
                txt += dic_entry;
                i++;
                c = code.substr(i, 1);
                txt += c;
                i+=1;
                aux = dic_entry + c;
                LZ78::Dict::add(aux, data_inv, data);
                // std::cout << LZ78::Dict::to_str(data_inv) << std::endl;
                break;
            }
            w = code.substr(i, l+2);
            i = i + l + 2;
            l = LZ78::int_decode(w, ab);
        }
    }
    return txt;
}

// int main() {
//     std::string w = "aabcbcbcbacbabcbabccbabb";
//     std::string ab = "abc";
//     std::string encoded = LZ78::encode(w, ab);
//     std::string decoded = LZ78::decode(encoded, ab);
//     std::cout << w << std::endl;
//     std::cout << encoded << std::endl;
//     std::cout << decoded << std::endl;
//     return 0;
// }
