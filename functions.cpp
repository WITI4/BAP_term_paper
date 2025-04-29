#include "input_check.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Hash {
    std::string hash;

    inline int receivingExistCodes(int x) {
        x += 256;
        while (!(((x <= 57) && (x >= 48)) || ((x <= 90) && (x >= 65)) || ((x <= 122) && (x >= 97)))) {
            if (x < 48) { x += 24; }
            else { x -= 47; }
        }
        return x;
    }
public:
    inline std::string GetHash(std::string str, unsigned hashLength) {
        hash.clear();

        if (hashLength < 4) {
            return "";
        }
        std::hash<std::string> hasher;
        size_t hashValue = hasher(str + "s@ltS1gn");

        for (unsigned i = 0; i < hashLength; i++) {
            int code = (hashValue >> (i * 8)) % 256;
            hash += receivingExistCodes(code);
            if (hash.size() >= hashLength) break;
        }

        return hash;
    }

    inline int GetControlSum(std::string str) {
        unsigned sault = 0;
        for (size_t i = 0; i < str.size(); i++) {
            sault += static_cast<unsigned char>(str[i]);
        }
        return sault;
    }
};

class User {
    std::string fullname;
    std::string username;
    std::string passwordHash;
    bool isBlocked;
public:
    User() : User("", "", "", 0) {}
    User(const User& other) : fullname(other.fullname), username(other.username), passwordHash(other.passwordHash), isBlocked(other.isBlocked) {}
    User(const std::string& fullname, const std::string& username, const std::string& passwordHash, bool isBlocked) {
        this->fullname = fullname;
        this->username = username;
        this->passwordHash = passwordHash;
        this->isBlocked = isBlocked;
    }

    std::string GetFullname() const { return fullname; }
    std::string GetUsername() const { return username; }
    std::string GetPasswordHash() const { return passwordHash; }
    constexpr bool GetIsBlocked() const { return isBlocked; }

    void SetFullname(const std::string& fullname) { this->fullname = fullname; }
    void SetUsername(const std::string& username) { this->username = username; }
    void SetPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }
    void SetIsBlocked(bool isBlocked) { this->isBlocked = isBlocked; }

    void saveToFile(std::ofstream& os) const {
        os << fullname << "\t"
           << username << "\t"
           << passwordHash << "\t"
           << (isBlocked ? "1" : "0") << "\n";
    }
};

class AuthSystem {
public:
    std::vector<User> users;
    Hash passwordHasher;

    bool userExists(const std::string& username) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i].GetUsername() == username) {
                return true;
            }
        }
        return false;
    }

    void loadUsers() {
        std::ifstream file("users.txt");
        std::string line;

        while (getline(file, line)) {
            int pos1 = line.find('\t');
            int pos2 = line.find('\t', pos1 + 1);
            int pos3 = line.find('\t', pos2 + 1);

            users.emplace_back(line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1, pos3 - pos2 - 1), line.substr(pos3 + 1) == "1");
        }
    }

    void saveUsers() {
        std::ofstream file("users.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи!\n";
            return;
        }

        for (int i = 0; i < users.size(); i++) {
            users[i].saveToFile(file);
        }
        file.close();
    }

    AuthSystem() {
        loadUsers();
    }

    ~AuthSystem() {
        saveUsers();
    }

    bool registerUser() {
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        std::string fullname, username, password;

        std::cout << "\nРЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ:\n\n";

        std::cout << "Ваше имя: ";
        letter_filteredInput<std::string>(fullname, 0, 1);

        std::cout << "Логин: ";
        letter_filteredInput<std::string>(username);

        if (userExists(username)) {
            std::cerr << "\nОшибка: Пользователь с таким логином уже существует!\n";
            system("pause");
            return false;
        }

            letter_filteredInput<std::string>(password, 0, 0, 1, 1);

        std::string hashedPassword = passwordHasher.GetHash(password, 16);
        users.emplace_back(fullname, username, hashedPassword, false);

        saveUsers();

        std::cerr << "\nПользователь успешно зарегистрирован!\n";
        system("pause");
        return true;
    }

    int loginUser() {
        std::string username, password;

        std::cout << "\nВХОД В АККАУНТ:\n\n";

        std::cout << "Логин: ";
        letter_filteredInput<std::string>(username);

        std::cout << "Пароль: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);

        std::string hashedPassword = passwordHasher.GetHash(password, 16);
        for (int i = 0; i < users.size(); i++) {
            if (users[i].GetUsername() == username && users[i].GetPasswordHash() == hashedPassword) {
                if (users[i].GetIsBlocked()) {
                    std::cerr << "\nОшибка: Аккаунт заблокирован!\n";
                    system("pause");
                    return -1;
                }
                return i;
            }
        }

        std::cerr << "\nОшибка: Неверный логин или пароль!\n";
        system("pause");
        return -1;
    }
};

static bool checkAdminPassword() {
    std::string password;
    std::cout << "Введите пароль администратора: ";
    letter_filteredInput<std::string>(password, 0, 0, 1);

    return password == "admin";
}