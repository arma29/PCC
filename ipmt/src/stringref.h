#ifndef STRING_REF
#define STRING_REF
#include <string>
#include <iostream>

class string_ref {

    std::string::iterator ite;

    std::string::iterator parent_begin;
    std::string::iterator parent_end;
    std::string::iterator sub_begin;
    std::string::iterator sub_end;

    int parent_length;
    int sub_start;
    int sub_length;

    bool valid=false;

    std::string proper_str;
    bool proper=false;
public:
    string_ref() {}

    string_ref(char * str) {
        proper_str = std::string("") + str;
        proper = true;
    }

    string_ref(std::string str,
               int ini,
               int length,
               bool proper = false) {

        if (proper) {
            proper_str = str.substr(ini, length);
            proper = true;
        }
        else {
            parent_begin = str.begin();
            parent_end = str.end();
            parent_length = parent_end - parent_begin;
            sub_length = length;
            sub_start = ini;
            sub_length = length;
            this->proper = false;

            if (ini + length > parent_length) {
                valid = false;
            }
            else {
                valid = true;
                sub_begin = parent_begin + ini;
                sub_end = parent_begin + ini + length;
            }
        }
    }

    string_ref(string_ref str,
               int ini,
               int length,
               bool proper = false) {
        if (str.isProper()) {
            string_ref(str.proper_str, ini, length, proper);
        } else {
            if (proper) {
                if (ini + length > str.length()) {
                    valid = false;
                } else {
                    proper_str = std::string(str.sub_begin + ini, str.sub_begin + ini + length);
                    this->proper = proper;
                }
            } else {
                parent_begin = str.begin();
                parent_end = str.end();
                parent_length = parent_end - parent_begin;
                sub_length = length;
                sub_start = ini;
                sub_length = length;
                this->proper = false;

                if (ini + length > parent_length) {
                    valid = false;
                }
                else {
                    valid = true;
                    sub_begin = parent_begin + ini;
                    sub_end = parent_begin + ini + length;
                }
            }
        }
    }

    string_ref(std::string str, bool proper = false) {
        string_ref(str, 0, str.end() - str.begin(), proper);
    }

    string_ref(string_ref &ref, bool proper = false) {
        if (ref.proper) {
            proper_str=ref.proper_str;
            this->proper=ref.proper;
        } else {
            if (proper) {
                proper_str = std::string(ref.sub_begin, ref.sub_end);
                this->proper = proper;
            } else {
                ite = ref.ite;
                parent_begin = ref.parent_begin;
                parent_end = ref.parent_end;
                sub_begin = ref.sub_begin;
                sub_end = ref.sub_end;

                parent_length = ref.parent_length;
                sub_start = ref.sub_start;
                sub_length = ref.sub_length;

                valid=ref.valid;
            }
        }
    }

    const std::string::iterator begin() {
        return sub_begin;
    }

    const std::string::iterator end() {
        return sub_end;
    }

    const bool isProper() {
        return proper;
    }

    const bool isValid() {
        return valid || proper;
    }

    const int length() {
        return sub_end - sub_begin;
    }

    bool operator ==(std::string& s) {
        if (proper) {
            return proper_str == s;
        }

        if (s.length() != sub_length || !isValid()) {
            std::cout << "Invalid comparison" << std::endl;
            return false;
        }
        else{
            ite = sub_begin;
            for (auto i = s.begin(); i != s.end();i++) {
                if (*i != *ite) {
                    return false;
                }
                ite++;
            }
        }
        return true;
    }

    bool operator ==(string_ref& s) {
        if (proper) {
            return s == proper_str;
        }

        if (s.sub_length != sub_length || !isValid()) {
            return false;
        }
        else{
            ite = sub_begin;
            for (auto i = s.sub_begin; i != s.sub_end;i++) {
                if (*i != *ite) {
                    return false;
                }
                ite++;
            }
        }
        return true;
    }

    const string_ref & operator +=(string_ref& str) {
        if (!isValid()) {
            std::cout << "Warning: Invalid string." << std::endl;
            proper_str = "";
            proper = true;
        }

        if (!proper) {
            proper_str = std::string(sub_begin, sub_end);
            proper = true;
        }

        if (str.proper) {
            proper_str += str.proper_str;
        } else {
            proper_str.append(str.begin(), str.end());
        }
        return *this;
    }

    const string_ref & operator +=(std::string& str) {
        if (!isValid()) {
            std::cout << "Warning: Invalid string." << std::endl;
            proper_str = "";
            proper = true;
        }

        if (!proper) {
            proper_str = std::string(sub_begin, sub_end);
            proper = true;
        }
        proper_str += str;
        return *this;
    }

    const string_ref & operator +=(char c) {
        if (!isValid()) {
            std::cout << "Warning: Invalid string." << std::endl;
            proper_str = "";
            proper = true;
        }
        
        if (!proper) {
            proper_str = std::string(sub_begin, sub_end);
            proper = true;
        }
        std::string aux(1, c);
        proper_str += aux;
        return *this;
    }

    friend const string_ref operator +(string_ref& str1, string_ref& str2) {
        string_ref result(str1, true);
        result += str2;
        return result;
    }

    friend const string_ref operator +(string_ref& str1, std::string& str2) {
        string_ref result(str1, true);
        result += str2;
        return result;
    }

    friend const string_ref operator +(std::string& str1, string_ref& str2) {
        string_ref result(str1, 0, str1.length());
        result += str2;
        return result;
    }

    friend const string_ref operator +(string_ref& str1, char str2) {
        string_ref result(str1, true);
        result += str2;
        return result;
    }

    friend const string_ref operator +(char str1, string_ref& str2) {
        std::string aux(1, str1);
        string_ref result(aux, 0, 1);
        result += str2;
        return result;
    }

    friend std::ostream & operator <<(std::ostream& stream, const string_ref & str ) {
        if (str.proper) {
            return stream << str.proper_str;
        } else {
            return stream << std::string(str.sub_begin, str.sub_end);
        }
    }

    const char operator [](int i){
        if (proper) {
            return proper_str[i];
        } else {
            return *(sub_begin + i);
        }
    }
};
#endif