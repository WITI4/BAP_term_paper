#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

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
void letter_filteredInput(std::string& buf, bool check_english = false, bool check_russian_english = false, bool mask_input = false, bool check_password_strength = false) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD originalAttrs = csbi.wAttributes;

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    int requirementsStartLine = 0;

    auto PrintPasswordRequirements = [&]() {
        if (!check_password_strength) return;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        COORD currentPos = csbi.dwCursorPosition;

        COORD pos = { 0, requirementsStartLine };
        SetConsoleCursorPosition(hConsole, pos);

        DWORD written;
        for (int i = 0; i < 5; i++) {
            FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, pos, &written);
            pos.Y++;
        }

        pos.Y = requirementsStartLine;
        SetConsoleCursorPosition(hConsole, pos);

        std::cout << "Требования к паролю (необходимо 3 из 4):\n";

        SetConsoleTextAttribute(hConsole, hasUpper ? FOREGROUND_GREEN : FOREGROUND_RED);
        std::cout << "- Заглавные буквы\n";
        SetConsoleTextAttribute(hConsole, hasLower ? FOREGROUND_GREEN : FOREGROUND_RED);
        std::cout << "- Строчные буквы\n";
        SetConsoleTextAttribute(hConsole, hasDigit ? FOREGROUND_GREEN : FOREGROUND_RED);
        std::cout << "- Цифры\n";
        SetConsoleTextAttribute(hConsole, hasSpecial ? FOREGROUND_GREEN : FOREGROUND_RED);
        std::cout << "- Специальные символы\n";

        SetConsoleTextAttribute(hConsole, originalAttrs);
        SetConsoleCursorPosition(hConsole, currentPos);
        };

    auto CheckPasswordValid = [&]() {
        int fulfilled = 0;
        if (hasUpper) fulfilled++;
        if (hasLower) fulfilled++;
        if (hasDigit) fulfilled++;
        if (hasSpecial) fulfilled++;
        return fulfilled >= 3;
        };

    if (check_password_strength) {
        std::cout << "\nТребования к паролю (необходимо 3 из 4):\n";
        std::cout << "- Заглавные буквы\n";
        std::cout << "- Строчные буквы\n";
        std::cout << "- Цифры\n";
        std::cout << "- Специальные символы\n";

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        requirementsStartLine = csbi.dwCursorPosition.Y - 5;

        std::cout << "\nПароль: ";
    }

    while (true) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {
            _getch();
            continue;
        }

        if (c == BACKSPACE) {
            if (!buf.empty()) {
                buf.pop_back();
                std::cout << "\b \b";

                if (check_password_strength) {
                    hasUpper = hasLower = hasDigit = hasSpecial = false;
                    for (char ch : buf) {
                        if (isupper(ch)) hasUpper = true;
                        if (islower(ch)) hasLower = true;
                        if (isdigit(ch)) hasDigit = true;
                        if (ispunct(ch)) hasSpecial = true;
                    }
                    PrintPasswordRequirements();
                }
            }
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
                if (!check_password_strength || CheckPasswordValid()) {
                    std::cout << std::endl;
                    return;
                }
            }
            continue;
        }

        if (buf.size() < MAX_DIGITS) {
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

            if (isValid) {
                buf += char(c);
                std::cout << (mask_input ? '*' : char(c));

                if (check_password_strength) {
                    char ch = char(c);
                    if (isupper(ch)) hasUpper = true;
                    if (islower(ch)) hasLower = true;
                    if (isdigit(ch)) hasDigit = true;
                    if (ispunct(ch)) hasSpecial = true;

                    PrintPasswordRequirements();
                }
            }
        }
    }
}