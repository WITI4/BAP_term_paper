#pragma once
#include <iostream>
#include <string>
#include <conio.h>

constexpr const int ENTER = 13;
constexpr const int BACKSPACE = 8;
constexpr const int MAX_DIGITS = 11;
const std::string MAX_INT32 = "2147483647";
const std::string MIN_INT32 = "2147483648";

double Stod(const std::string& s);

bool check_unsigned_input(const std::string& s);
bool check_int_input(const std::string& s);
bool check_onlyNegative_int_input(const std::string& s);
bool check_onlyPositive_int_input(const std::string& s);
bool check_double_float_input(const std::string& s);

bool check_englishLetters_specSymbols_input(const std::string& s);
bool check_russian_englishLetters_input(const std::string& s);
bool check_englishLetters_input(const std::string& s);

template <typename T>
bool number_filteredInput(T& result, bool requireNegative = false, bool requirePositive = false) {
    std::string buf;

    while (true) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {
            _getch();
            continue;
        }

        if (c == BACKSPACE && !buf.empty()) {
            buf.pop_back();
            std::cout << "\b \b";
            continue;
        }

        if (c == ENTER && !buf.empty()) {
            try {
                if constexpr (std::is_same_v<T, unsigned>) {
                    if (check_unsigned_input(buf)) {
                        result = std::stoul(buf);
                        std::cout << std::endl;
                        return true;
                    }
                }
                else if constexpr (std::is_same_v<T, int>) {
                    if ((requireNegative && buf[0] != '-') ||
                        (requirePositive && !buf.empty() && buf[0] == '-')) {
                        continue;
                    }
                    if (check_int_input(buf)) {
                        result = std::stoi(buf);
                        std::cout << std::endl;
                        return true;
                    }
                }
                else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
                    if (check_double_float_input(buf)) {
                        result = Stod(buf);
                        std::cout << std::endl;
                        return true;
                    }
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    result = buf;
                    std::cout << std::endl;
                    return true;
                }
            }
            catch (...) {
                return false;
            }
        }

        if constexpr (std::is_same_v<T, unsigned>) {
            if (isdigit(c) && buf.size() < MAX_DIGITS) {
                std::string new_buf = buf + char(c);
                if (check_unsigned_input(new_buf)) {
                    buf += char(c);
                    std::cout << char(c);
                }
            }
        }
        else if constexpr (std::is_same_v<T, int>) {
            if ((isdigit(c) || (c == '-' && buf.empty())) && buf.size() < MAX_DIGITS) {
                std::string new_buf = (c == '-') ? "-" : buf + char(c);
                bool isValid = false;

                if (requireNegative) {
                    isValid = (c == '-') || check_onlyNegative_int_input(new_buf);
                }
                else if (requirePositive) {
                    isValid = (c != '-') && check_onlyPositive_int_input(new_buf);
                }
                else {
                    isValid = check_int_input(new_buf);
                }

                if (isValid) {
                    buf += char(c);
                    std::cout << char(c);
                }
            }
        }
        else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
            if ((isdigit(c) || c == '.' || (c == '-' && buf.empty())) && buf.size() < MAX_DIGITS) {
                std::string new_buf = buf + static_cast<char>(c);
                if (check_double_float_input(new_buf)) {
                    buf += static_cast<char>(c);
                    std::cout << static_cast<char>(c);
                }
            }
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            if (c >= 32 && c <= 126 && buf.size() < MAX_DIGITS) {
                buf += static_cast<char>(c);
                std::cout << static_cast<char>(c);
            }
        }
    }
}

template <typename T>
void letter_filteredInput(std::string& buf, bool check_english = false, bool check_russian_english = false, bool mask_input = false) {
    buf.clear();

    while (true) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {
            _getch();
            continue;
        }

        if (c == BACKSPACE && !buf.empty()) {
            buf.pop_back();
            std::cout << "\b \b";
            continue;
        }

        if (c == ENTER && !buf.empty()) {
            if constexpr (std::is_same_v<T, unsigned>) {
                if (buf.size() <= MAX_DIGITS && check_unsigned_input(buf)) {
                    std::cout << std::endl;
                    return;
                }
            }
            else if constexpr (std::is_same_v<T, int>) {
                if (buf.size() <= MAX_DIGITS && check_int_input(buf)) {
                    std::cout << std::endl;
                    return;
                }
            }
            else {
                std::cout << std::endl;
                return;
            }
        }

        if constexpr (std::is_same_v<T, std::string>) {
            std::string charAsString(1, char(c));
            bool isValid = false;

            if (check_english) {
                isValid = check_englishLetters_input(charAsString);
            }
            else if (check_russian_english) {
                isValid = check_russian_englishLetters_input(charAsString);
            }
            else {
                isValid = check_englishLetters_specSymbols_input(charAsString);
            }

            if (isValid && buf.size() < MAX_DIGITS) {
                buf += char(c);
                if (mask_input) {
                    std::cout << '*';
                }
                else {
                    std::cout << char(c);
                }
            }
        }
    }
}