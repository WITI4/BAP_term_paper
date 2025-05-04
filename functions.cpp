#include "input_check.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <functional>

enum FilterMenu { FILTER_CHANNEL, FILTER_PROGRAM, FILTER_DATE, FILTER_TIME, FILTER_DURATION, FILTER_DAYS, FILTER_SHOWS_PER_DAY, FILTER_PRIME_TIME, FILTER_POSITION, FILTER_PRICE, FILTER_CONFIRM, FILTER_GO_BACK, FILTER_COUNT };

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

class AdTime {
    std::string channel;
    std::string program;
    std::string date;
    std::string startTime;
    std::string endTime;
    int adDuration;
    int daysToShow;
    int showsPerDay;
    bool isPrimeTime;
    std::string adPosition;
    double price;

public:
    AdTime() : AdTime("", "", "", "", "", 0, 0, 0, false, "", 0.0) {}
    AdTime(const AdTime& other) : channel(other.channel), program(other.program), date(other.date), startTime(other.startTime), endTime(other.endTime), adDuration(other.adDuration), daysToShow(other.daysToShow), showsPerDay(other.showsPerDay), isPrimeTime(other.isPrimeTime), adPosition(other.adPosition), price(other.price) {}
    AdTime(const std::string& channel, const std::string& program, const std::string& date, const std::string& startTime, const std::string& endTime, int adDuration, int daysToShow, int showsPerDay, bool isPrimeTime, const std::string& adPosition, double price) {
        this->channel = channel;
        this->program = program;
        this->date = date;
        this->startTime = startTime;
        this->endTime = endTime;
        this->adDuration = adDuration;
        this->daysToShow = daysToShow;
        this->showsPerDay = showsPerDay;
        this->isPrimeTime = isPrimeTime;
        this->adPosition = adPosition;
        this->price = price;
    }

    std::string GetChannel() const { return channel; }
    std::string GetProgram() const { return program; }
    std::string GetDate() const { return date; }
    std::string GetStartTime() const { return startTime; }
    std::string GetEndTime() const { return endTime; }
    constexpr int GetAdDuration() const { return adDuration; }
    constexpr int GetDaysToShow() const { return daysToShow; }
    constexpr int GetShowsPerDay() const { return showsPerDay; }
    constexpr bool GetIsPrimeTime() const { return isPrimeTime; }
    std::string GetAdPosition() const { return adPosition; }
    constexpr double GetPrice() const { return price; }

    void SetChannel(const std::string& channel) { this->channel = channel; }
    void SetProgram(const std::string& program) { this->program = program; }
    void SetDate(const std::string& date) { this->date = date; }
    void SetStartTime(const std::string& startTime) { this->startTime = startTime; }
    void SetEndTime(const std::string& endTime) { this->endTime = endTime; }
    void SetAdDuration(int adDuration) { this->adDuration = adDuration; }
    void SetDaysToShow(int daysToShow) { this->daysToShow = daysToShow; }
    void SetShowsPerDay(int showsPerDay) { this->showsPerDay = showsPerDay; }
    void SetIsPrimeTime(bool isPrimeTime) { this->isPrimeTime = isPrimeTime; }
    void SetAdPosition(const std::string& adPosition) { this->adPosition = adPosition; }
    void SetPrice(double price) { this->price = price; }

    void saveToFile(std::ofstream& os) const {
        os << channel << "\t"
            << program << "\t"
            << date << "\t"
            << startTime << "\t"
            << endTime << "\t"
            << adDuration << "\t"
            << daysToShow << "\t"
            << showsPerDay << "\t"
            << (isPrimeTime ? "1" : "0") << "\t"
            << adPosition << "\t"
            << price << "\n";
    }
};

class AuthSystem {
public:
    std::vector<User> users;
    std::vector<AdTime> adTimes;
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
        users.clear();
        std::ifstream file("users.txt");
        std::string line;

        while (getline(file, line)) {
            int pos1 = line.find('\t');
            int pos2 = line.find('\t', pos1 + 1);
            int pos3 = line.find('\t', pos2 + 1);

            users.emplace_back(line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1, pos3 - pos2 - 1), line.substr(pos3 + 1) == "1");
        }
    }

    void loadAdTimes() {
        adTimes.clear();
        std::ifstream file("adtimes.txt");
        std::string line;

        while (getline(file, line)) {
            int pos1 = line.find('\t');
            int pos2 = line.find('\t', pos1 + 1);
            int pos3 = line.find('\t', pos2 + 1);
            int pos4 = line.find('\t', pos3 + 1);
            int pos5 = line.find('\t', pos4 + 1);
            int pos6 = line.find('\t', pos5 + 1);
            int pos7 = line.find('\t', pos6 + 1);
            int pos8 = line.find('\t', pos7 + 1);
            int pos9 = line.find('\t', pos8 + 1);
            int pos10 = line.find('\t', pos9 + 1);

            adTimes.emplace_back(line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1, pos3 - pos2 - 1), line.substr(pos3 + 1, pos4 - pos3 - 1), line.substr(pos4 + 1, pos5 - pos4 - 1), std::stoi(line.substr(pos5 + 1, pos6 - pos5 - 1)), std::stoi(line.substr(pos6 + 1, pos7 - pos6 - 1)), std::stoi(line.substr(pos7 + 1, pos8 - pos7 - 1)), line.substr(pos8 + 1, pos9 - pos8 - 1) == "1", line.substr(pos9 + 1, pos10 - pos9 - 1), std::stod(line.substr(pos10 + 1)));
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

    void saveAdTimes() {
        std::ofstream file("adtimes.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка при открытии файла для записи!\n";
            return;
        }

        for (int i = 0; i < adTimes.size(); i++) {
            adTimes[i].saveToFile(file);
        }
        file.close();
    }

    AuthSystem() {
        loadUsers();
        loadAdTimes();
    }

    ~AuthSystem() {
        saveUsers();
        saveAdTimes();
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

class AdTimeManager {
public:
    static void printAdTimesTable(const std::vector<AdTime>& adTimes, int startIndex = 1) {
        std::cout << "----------------------------------------------------------------------------------------------------------\n";
        std::cout << "| №  | Канал  | Программа         | Дата      | Время      | Дл | Дн |Раз в | Прайм | Позиция   | Цена   |\n";
        std::cout << "|    |        |                   |           |            | ит |    |день  | тайм  |           |        |\n";
        std::cout << "----------------------------------------------------------------------------------------------------------\n";

        int index = startIndex;
        for (int i = 0; i < adTimes.size(); i++) {
            std::cout << "| " << std::setw(3) << std::left << index++ << "| ";
            std::cout << std::setw(7) << std::left << adTimes[i].GetChannel() << "| ";
            std::cout << std::setw(18) << std::left << adTimes[i].GetProgram() << "| ";
            std::cout << std::setw(8) << std::left << adTimes[i].GetDate() << "| ";
            std::cout << std::setw(10) << std::left << (adTimes[i].GetStartTime() + "-" + adTimes[i].GetEndTime()) << "| ";
            std::cout << std::setw(3) << std::left << adTimes[i].GetAdDuration() << "| ";
            std::cout << std::setw(3) << std::left << adTimes[i].GetDaysToShow() << "| ";
            std::cout << std::setw(5) << std::left << adTimes[i].GetShowsPerDay() << "| ";
            std::cout << std::setw(6) << std::left << (adTimes[i].GetIsPrimeTime() ? "Да" : "Нет") << "| ";
            std::cout << std::setw(10) << std::left << adTimes[i].GetAdPosition() << "| ";
            std::cout << std::setw(7) << std::left << adTimes[i].GetPrice() << "|\n";
        }
        std::cout << "----------------------------------------------------------------------------------------------------------\n";
    }

    static void showSelectedColumns(const std::vector<AdTime>& adTimes, const std::vector<bool>& selectedColumns) {
        system("cls");

        std::cout << "----------------------------------------------------------------------------------------------------------\n";
        std::cout << "| №  |";
        if (selectedColumns[FILTER_CHANNEL]) std::cout << " Канал  |";
        if (selectedColumns[FILTER_PROGRAM]) std::cout << " Программа         |";
        if (selectedColumns[FILTER_DATE]) std::cout << " Дата       |";
        if (selectedColumns[FILTER_TIME]) std::cout << " Время      |";
        if (selectedColumns[FILTER_DURATION]) std::cout << " Дл |";
        if (selectedColumns[FILTER_DAYS]) std::cout << " Дн |";
        if (selectedColumns[FILTER_SHOWS_PER_DAY]) std::cout << "Раз в |";
        if (selectedColumns[FILTER_PRIME_TIME]) std::cout << " Прайм |";
        if (selectedColumns[FILTER_POSITION]) std::cout << " Позиция   |";
        if (selectedColumns[FILTER_PRICE]) std::cout << " Цена   |";
        std::cout << "\n";

        std::cout << "|    |";
        if (selectedColumns[FILTER_CHANNEL]) std::cout << "        |";
        if (selectedColumns[FILTER_PROGRAM]) std::cout << "                   |";
        if (selectedColumns[FILTER_DATE]) std::cout << "            |";
        if (selectedColumns[FILTER_TIME]) std::cout << "            |";
        if (selectedColumns[FILTER_DURATION]) std::cout << " ит |";
        if (selectedColumns[FILTER_DAYS]) std::cout << "    |";
        if (selectedColumns[FILTER_SHOWS_PER_DAY]) std::cout << "день  |";
        if (selectedColumns[FILTER_PRIME_TIME]) std::cout << " тайм  |";
        if (selectedColumns[FILTER_POSITION]) std::cout << "           |";
        if (selectedColumns[FILTER_PRICE]) std::cout << "        |";
        std::cout << "\n";
        std::cout << "----------------------------------------------------------------------------------------------------------\n";

        int index = 1;
        for (int i = 0; i < adTimes.size(); i++) {
            std::cout << "| " << std::setw(3) << std::left << index++ << "|";
            if (selectedColumns[FILTER_CHANNEL]) std::cout << " " << std::setw(7) << std::left << adTimes[i].GetChannel() << "|";
            if (selectedColumns[FILTER_PROGRAM]) std::cout << " " << std::setw(18) << std::left << adTimes[i].GetProgram() << "|";
            if (selectedColumns[FILTER_DATE]) std::cout << " " << std::setw(8) << std::left << adTimes[i].GetDate() << " |";
            if (selectedColumns[FILTER_TIME]) std::cout << " " << std::setw(10) << std::left << (adTimes[i].GetStartTime() + "-" + adTimes[i].GetEndTime()) << "|";
            if (selectedColumns[FILTER_DURATION]) std::cout << " " << std::setw(3) << std::left << adTimes[i].GetAdDuration() << "|";
            if (selectedColumns[FILTER_DAYS]) std::cout << " " << std::setw(3) << std::left << adTimes[i].GetDaysToShow() << "|";
            if (selectedColumns[FILTER_SHOWS_PER_DAY]) std::cout << " " << std::setw(5) << std::left << adTimes[i].GetShowsPerDay() << "|";
            if (selectedColumns[FILTER_PRIME_TIME]) std::cout << " " << std::setw(6) << std::left << (adTimes[i].GetIsPrimeTime() ? "Да" : "Нет") << "|";
            if (selectedColumns[FILTER_POSITION]) std::cout << " " << std::setw(10) << std::left << adTimes[i].GetAdPosition() << "|";
            if (selectedColumns[FILTER_PRICE]) std::cout << " " << std::setw(7) << std::left << adTimes[i].GetPrice() << "|";
            std::cout << "\n";
        }
        std::cout << "----------------------------------------------------------------------------------------------------------\n";
        system("pause");
    }

    static void showTop3ByAdDuration(const std::vector<AdTime>& adTimes) {
        std::vector<AdTime> summedAdTimes;

        for (int i = 0; i < adTimes.size(); i++) {
            const AdTime& currentAd = adTimes[i];
            bool found = false;

            for (int j = 0; j < summedAdTimes.size(); j++) {
                AdTime& summedAd = summedAdTimes[j];
                if (summedAd.GetProgram() == currentAd.GetProgram() &&
                    summedAd.GetChannel() == currentAd.GetChannel()) {

                    summedAd.SetAdDuration(summedAd.GetAdDuration() + currentAd.GetAdDuration());
                    found = true;
                    break;
                }
            }

            if (!found) {
                summedAdTimes.push_back(currentAd);
            }
        }

        int n = summedAdTimes.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (summedAdTimes[j].GetAdDuration() < summedAdTimes[j + 1].GetAdDuration()) {
                    std::swap(summedAdTimes[j], summedAdTimes[j + 1]);
                }
            }
        }

        system("cls");
        std::cout << "ТОП-3 передачи с максимальным временем рекламы:\n";
        std::cout << "------------------------------------------------------------\n";
        std::cout << "| №  | Канал  | Программа         | Время рекламы (мин) |\n";
        std::cout << "------------------------------------------------------------\n";

        for (int i = 0; i < 3; i++) {
            const AdTime& ad = summedAdTimes[i];
            std::cout << "| " << std::setw(3) << std::left << i + 1 << "| ";
            std::cout << std::setw(7) << std::left << ad.GetChannel() << "| ";
            std::cout << std::setw(18) << std::left << ad.GetProgram() << "| ";
            std::cout << std::setw(20) << std::left << ad.GetAdDuration() << "|\n";
        }
        std::cout << "------------------------------------------------------------\n";
        system("pause");
    }

    static void bubbleSort(std::vector<AdTime>& adTimes, int sortField) {
        int n = adTimes.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                bool needSwap = false;

                switch (sortField) {
                case FILTER_CHANNEL:
                    needSwap = adTimes[j].GetChannel() > adTimes[j + 1].GetChannel();
                    break;
                case FILTER_PROGRAM:
                    needSwap = adTimes[j].GetProgram() > adTimes[j + 1].GetProgram();
                    break;
                case FILTER_DATE:
                    needSwap = adTimes[j].GetDate() > adTimes[j + 1].GetDate();
                    break;
                case FILTER_TIME:
                    needSwap = adTimes[j].GetStartTime() > adTimes[j + 1].GetStartTime();
                    break;
                case FILTER_DURATION:
                    needSwap = adTimes[j].GetAdDuration() > adTimes[j + 1].GetAdDuration();
                    break;
                case FILTER_DAYS:
                    needSwap = adTimes[j].GetDaysToShow() > adTimes[j + 1].GetDaysToShow();
                    break;
                case FILTER_SHOWS_PER_DAY:
                    needSwap = adTimes[j].GetShowsPerDay() > adTimes[j + 1].GetShowsPerDay();
                    break;
                case FILTER_PRIME_TIME:
                    needSwap = adTimes[j].GetIsPrimeTime() > adTimes[j + 1].GetIsPrimeTime();
                    break;
                case FILTER_POSITION:
                    needSwap = adTimes[j].GetAdPosition() > adTimes[j + 1].GetAdPosition();
                    break;
                case FILTER_PRICE:
                    needSwap = adTimes[j].GetPrice() > adTimes[j + 1].GetPrice();
                    break;
                }

                if (needSwap) {
                    std::swap(adTimes[j], adTimes[j + 1]);
                }
            }
        }
    }
};

static bool checkAdminPassword() {
    std::string password;
    std::cout << "Введите пароль администратора: ";
    letter_filteredInput<std::string>(password, 0, 0, 1);
    return password == "admin";
}

static std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}
static bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

static int getDaysInMonth(int month, int year) {
    switch (month) {
    case 2:  return isLeapYear(year) ? 29 : 28;
    case 4: case 6: case 9: case 11: return 30;
    default: return 31;
    }
}

static bool isValidDate(int day, int month, int year) {
    if (year < 1) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(month, year)) return false;
    return true;
}