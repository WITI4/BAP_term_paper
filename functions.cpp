#include "input_check.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Hash {
public:
    std::string GetHash(std::string str, unsigned hashLenght);
    int GetControlSum(std::string str);

private:
    std::string hash;
    int receivingExistCodes(int x);
};

int Hash::receivingExistCodes(int x) {
    x += 256;
    while (!(((x <= 57) && (x >= 48)) || ((x <= 90) && (x >= 65)) || ((x <= 122) && (x >= 97)))) {
        if (x < 48) { x += 24; }
        else { x -= 47; }
    }
    return x;
}

int Hash::GetControlSum(std::string str) {
    unsigned sault = 0;
    for (size_t i = 0; i < str.size(); i++) {
        sault += static_cast<unsigned char>(str[i]);
    }
    return sault;
}

std::string Hash::GetHash(std::string str, unsigned hashLenght) {
    if (hashLenght < 4) {
        return " ";
    }

    unsigned minLength = 2;
    const unsigned originalLenghtStr = str.size();

    if (originalLenghtStr == 0) {
        return " ";
    }

    while (minLength < originalLenghtStr) {
        minLength *= 2;
    }
    if ((minLength - originalLenghtStr) < minLength) {
        minLength *= 2;
    }

    const int addCount = minLength - originalLenghtStr;
    for (int i = 0; i < addCount; i++) {
        const size_t idx1 = i % originalLenghtStr;
        const size_t idx2 = (i + 1) % originalLenghtStr;
        str += receivingExistCodes(str[idx1] + str[idx2]);
    }

    const unsigned originalSault = GetControlSum(str);
    const unsigned maxLenghtStr = str.size();
    const int maxSault = GetControlSum(str);

    std::string tempHash;
    while (str.size() > hashLenght) {
        tempHash.clear();
        const size_t center = str.size() / 2;

        for (size_t i = 0; i < center; i++) {
            const size_t left = center - i - 1;
            size_t right = center + i;
            if (right >= str.size()) {
                right = str.size() - 1;
            }
            tempHash += receivingExistCodes(str[left] + str[right]);
        }
        str = tempHash;
    }

    hash.clear();
    for (size_t i = 0; hash.size() < hashLenght - 4 && i < str.size(); i++) {
        hash += str[i];
    }

    hash += receivingExistCodes(originalSault);
    hash += receivingExistCodes(originalLenghtStr);
    hash += receivingExistCodes(maxSault);
    hash += receivingExistCodes(maxLenghtStr);

    if (hash.size() > hashLenght) {
        hash.resize(hashLenght);
    }

    return hash;
}

class User {
    std::string fullname;
    std::string username;
    std::string passwordHash;
    bool isBlocked;
public:
    User() : User("", "", "", false) {}
    User(const std::string& fullname, const std::string& username, const std::string& passwordHash, bool isBlocked) {
        this->fullname = fullname;
        this->username = username;
        this->passwordHash = passwordHash;
        this->isBlocked = isBlocked;
    }

    std::string GetFullname() const { return fullname; }
    std::string GetUsername() const { return username; }
    std::string GetPasswordHash() const { return passwordHash; }
    bool GetIsBlocked() const { return isBlocked; }

    void SetFullname(const std::string& fullname) { this->fullname = fullname; }
    void SetUsername(const std::string& username) { this->username = username; }
    void SetPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }
    void SetIsBlocked(bool isBlocked) { this->isBlocked = isBlocked; }

    void saveToFile(std::ofstream& file) const {
        file << fullname << "\n"
            << username << "\n"
            << passwordHash << "\n"
            << isBlocked << "\n";
    }
};

class AuthSystem {
public:
    std::vector<User> users;
    const std::string usersFile = "users.txt";
    Hash passwordHasher;

    bool userExists(const std::string& username) {
        for (const auto& user : users) {
            if (user.GetUsername() == username) {
                return true;
            }
        }
        return false;
    }

    void loadUsers() {
        std::ifstream file(usersFile);
        if (!file.is_open()) return;

        std::string fullname, username, passwordHash;
        bool isBlocked;

        while (getline(file, fullname)) {
            getline(file, username);
            getline(file, passwordHash);
            file >> isBlocked;
            file.ignore();

            users.emplace_back(fullname, username, passwordHash, isBlocked);
        }
        file.close();
    }

    void saveUsers() {
        std::ofstream file(usersFile);
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи!\n";
            return;
        }

        for (const auto& user : users) {
            user.saveToFile(file);
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

        std::cout << "Пароль: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);

        std::string hashedPassword = passwordHasher.GetHash(password, 32);

        users.emplace_back(fullname, username, hashedPassword, false);
        saveUsers();

        std::cerr << "\nПользователь успешно зарегистрирован!\n";
        system("pause");
        return true;
    }

    User* loginUser() {
        std::string username, password;

        std::cout << "\nВХОД В АККАУНТ:\n\n";

        std::cout << "Логин: ";
        letter_filteredInput<std::string>(username);

        std::cout << "Пароль: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);

        std::string hashedPassword = passwordHasher.GetHash(password, 32);

        for (auto& user : users) {
            if (user.GetUsername() == username && user.GetPasswordHash() == hashedPassword) {
                if (user.GetIsBlocked()) {
                    std::cerr << "\nОшибка: Аккаунт заблокирован!\n";
                    system("pause");
                    return nullptr;
                }
                return &user;
            }
        }

        std::cerr << "\nОшибка: Неверный логин или пароль!\n";
        system("pause");
        return nullptr;
    }
};

static bool checkAdminPassword() {
    std::string password;
    std::cout << "Введите пароль администратора: ";
    letter_filteredInput<std::string>(password, 0, 0, 1);

    return password == "admin";
}