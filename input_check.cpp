#include"input_check.h"

double Stod(const std::string& s) {
    size_t i = 0;
    bool negative = false;

    while (i < s.size() && isspace(s[i])) i++;

    if (i < s.size() && s[0] == '-') {
        negative = true;
        i++;
    }

    double result = 0;
    while (i < s.size() && isdigit(s[i])) {
        result = result * 10 + (s[i] - '0');
        i++;
    }

    if (i < s.size() && s[i] == '.') {
        i++;
        double fraction = 0;
        double divisor = 1;

        while (i < s.size() && isdigit(s[i])) {
            fraction = fraction * 10 + (s[i] - '0');
            divisor *= 10;
            i++;
        }
        result += fraction / divisor;
    }

    return negative ? -result : result;
}

bool check_unsigned_input(const std::string& s) {
    if (s.empty() || s.size() > MAX_DIGITS) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return s.size() < MAX_INT32.size() || (s.size() == MAX_INT32.size() && s <= MAX_INT32);
}
bool check_int_input(const std::string& s) {
    if (s.empty() || s.size() > MAX_DIGITS) return false;
    size_t i = 0;
    bool negative = false;

    if (s[0] == '-') {
        i++;
        negative = true;
    }
    for (; i < s.size(); i++) if (!isdigit(s[i])) return false;

    if (negative) {
        return s.size() - 1 < MIN_INT32.size() || (s.size() - 1 == MIN_INT32.size() && s.substr(1) <= MIN_INT32);
    }
    return s.size() < MAX_INT32.size() || (s.size() == MAX_INT32.size() && s <= MAX_INT32);
}
bool check_onlyNegative_int_input(const std::string& s) {
    if (s.empty() || s[0] != '-') return false;
    std::string num = s.substr(1);
    return num.length() < MIN_INT32.length() || (num.length() == MIN_INT32.length() && num <= MIN_INT32);
}
bool check_onlyPositive_int_input(const std::string& s) {
    if (s.empty()) return false;
    if (s[0] == '-') return false;
    if (!(s.begin(), s.end(), ::isdigit)) return false;
    return s.length() < MAX_INT32.length() || (s.length() == MAX_INT32.length() && s <= MAX_INT32);
}
bool check_double_float_input(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    bool dotFound = false;

    if (s[0] == '-') i++;

    for (; i < s.size(); i++) {
        if (s[i] == '.') {
            if (dotFound) return false;
            dotFound = true;
        }
        else if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

bool check_englishLetters_specSymbols_input(const std::string& s) {
    if (s.empty()) return false;
    char c = s[0];
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-') || (c == '_') || (c == '.');
}
bool check_russian_englishLetters_input(const std::string& s) {
    if (s.empty()) return false;
    char c = s[0];
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' || c >= 'à' && c <= 'ÿ') || (c >= 'À' && c <= 'ß');
}
bool check_englishLetters_input(const std::string& s) {
    if (s.empty()) return false;
    char c = s[0];
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


//==============DEBUG==============
/*
unsigned a;
int b;
double value;
float d;
std::string str;

std::cout << "<unsigned>: ";
number_filteredInput<unsigned>(a);
std::cout << a << "\n\n";

std::cout << "<int>: ";
number_filteredInput<int>(b);
std::cout << b << "\n\n";

std::cout << "<-int>: ";
number_filteredInput<int>(b, 1);
std::cout << b << "\n\n";

std::cout << "<+int>: ";
number_filteredInput<int>(b, 0, 1);
std::cout << b << "\n\n";

std::cout << "<double>: ";
number_filteredInput<double>(value);
std::cout << value << "\n\n";

std::cout << "<float>: ";
number_filteredInput<float>(d);
std::cout << d << "\n\n\n\n";


std::cout << "<string (english + '-' +'_' + 1-9)>: ";
letter_filteredInput<std::string>(str);
std::cout << str << "\n\n";

std::cout << "<only english>: ";
letter_filteredInput<std::string>(str, 1);
std::cout << str << "\n\n";

std::cout << "<english + russian>: ";
letter_filteredInput<std::string>(str, 0, 1);
std::cout << str << "\n\n";

std::cout << "<*>: ";
letter_filteredInput<std::string>(str, 0, 0, 1);
std::cout << str << "\n\n";
*/