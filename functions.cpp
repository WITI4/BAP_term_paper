#include "input_check.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class User {
    std::string fullname;
    std::string username;
    std::string password;
    bool isBlocked;
public:
    User() : User("", "", "", false) {}
    User(const std::string& fullname, const std::string& username,
        const std::string& password, bool isBlocked) :
        fullname(fullname), username(username), password(password), isBlocked(isBlocked) {
    }

    std::string GetFullname() const { return fullname; }
    std::string GetUsername() const { return username; }
    std::string GetPassword() const { return password; }
    bool GetIsBlocked() const { return isBlocked; }

    void SetFullname(const std::string& fullname) { this->fullname = fullname; }
    void SetUsername(const std::string& username) { this->username = username; }
    void SetPassword(const std::string& password) { this->password = password; }
    void SetIsBlocked(bool isBlocked) { this->isBlocked = isBlocked; }

    void saveToFile(std::ofstream& file) const {
        file << fullname << "\n"
            << username << "\n"
            << password << "\n"
            << isBlocked << "\n";
    }
};

class AuthSystem {
private:
    std::vector<User> users;
    const std::string usersFile = "users.txt";

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

        std::string fullname, username, password;
        bool isBlocked;

        while (getline(file, fullname)) {
            getline(file, username);
            getline(file, password);
            file >> isBlocked;
            file.ignore();

            users.emplace_back(fullname, username, password, isBlocked);
        }
        file.close();
    }

    void saveUsers() {
        std::ofstream file(usersFile);
        if (!file.is_open()) {
            std::cerr << "������ ��� �������� ����� ��� ������!\n";
            return;
        }

        for (const auto& user : users) {
            user.saveToFile(file);
        }
        file.close();
    }

public:
    AuthSystem() {
        loadUsers();
    }

    ~AuthSystem() {
        saveUsers();
    }

    bool registerUser() {
        std::string fullname, username, password;

        std::cout << "\n����������� ������������:\n\n";

        std::cout << "���� ���: ";
        letter_filteredInput<std::string>(fullname, 0, 1);

        std::cout << "�����: ";
        letter_filteredInput<std::string>(username);

        if (userExists(username)) {
            std::cerr << "\n������: ������������ � ����� ������� ��� ����������!\n";
            system("pause");
            return false;
        }

        std::cout << "������: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);

        users.emplace_back(fullname, username, password, false);
        saveUsers();

        std::cerr << "\n������������ ������� ���������������!\n";
        system("pause");
        return true;
    }

    User* loginUser() {
        std::string username, password;

        std::cout << "\n���� � �������:\n\n";

        std::cout << "�����: ";
        letter_filteredInput<std::string>(username);

        std::cout << "������: ";
        letter_filteredInput<std::string>(password, 0, 0, 1);

        for (auto& user : users) {
            if (user.GetUsername() == username && user.GetPassword() == password) {
                if (user.GetIsBlocked()) {
                    std::cerr << "\n������: ������� ������������!\n";
                    system("pause");
                    return nullptr;
                }
                return &user;
            }
        }

        std::cerr << "\n������: �������� ����� ��� ������!\n";
        system("pause");
        return nullptr;
    }
};
static bool checkAdminPassword() {
    std::string password;
    std::cout << "������� ������ ��������������: ";
    letter_filteredInput<std::string>(password, 0, 0, 1);

    return password == "admin";
}