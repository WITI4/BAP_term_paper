#include "cursor_visibility.h"
#include "cursor_menu.h"
#include "console_inactivity_timer.h"
#include "input_check.h"
#include "smoothWink.h"
#include "functions.cpp"
#include <iostream>
#include <windows.h>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    std::thread timer(inactivity_timer, 100);
    timer.detach();

    enum FirstLevelMenu { MENU_USER, MENU_ADMIN, EXIT_PROGRAM };
    enum RegistrationMenu { MENU_AUTH_REGISTER, MENU_AUTH_LOGIN, MENU_AUTH_GO_BACK };
    enum UserMenu { USER_VIEW_TABLE, USER_SEARCH, USER_FILTER, USER_CUSTOM_TASK, USER_GO_BACK, USER_EXIT };
    enum UserManagementMenu { USER_MGMT_VIEW_ALL, USER_MGMT_BLOCK, USER_MGMT_UNBLOCK, USER_MGMT_DELETE, USER_MGMT_GO_BACK };
    enum AdminMenu { ADMIN_VIEW_TABLE, ADMIN_ADD_RECORD, ADMIN_EDIT_RECORD, ADMIN_DELETE_RECORD, ADMIN_SEARCH, ADMIN_FILTER, ADMIN_SORT, ADMIN_CUSTOM_TASK, ADMIN_MANAGE_USERS, ADMIN_GO_BACK, ADMIN_EXIT };
    
    AuthSystem authSystem;
    authSystem.loadAdTimes();

    while (true) {
        hideCursor();

        // Главное меню
        const std::string firstLevelMenu[]{
            "Пользователь",
            "Администратор",
            "Выход из программы"
        };
        const int firstLevelMenu_Count = sizeof(firstLevelMenu) / sizeof(firstLevelMenu[0]);

        int firstLevelMenu_choice = main_showMenu(".../firstLevelMenu/", firstLevelMenu, firstLevelMenu_Count, 1);

        switch (firstLevelMenu_choice) {
        case MENU_USER: {
            bool shouldReturnToFirstLevelMenu = false;
            while (!shouldReturnToFirstLevelMenu) {
                hideCursor();

                // Меню регистрации/авторизации
                const std::string registrationMenu[]{
                    "Зарегистрироваться",
                    "Войти в аккаунт",
                    "Вернуться назад"
                };
                const int registrationMenu_Count = sizeof(registrationMenu) / sizeof(registrationMenu[0]);

                int registrationMenu_choice = main_showMenu(".../firstLevelMenu/registrationMenu/USER/", registrationMenu, registrationMenu_Count);

                switch (registrationMenu_choice) {
                case MENU_AUTH_REGISTER: {
                    authSystem.registerUser();
                    break;
                }
                case MENU_AUTH_LOGIN: {
                    int userIndex = authSystem.loginUser();
                    if (userIndex != -1) {
                        std::cout << std::endl << "Добро пожаловать, " << authSystem.users[userIndex].GetFullname() << "!" << std::endl;
                        system("pause");

                        while (!shouldReturnToFirstLevelMenu) {
                            hideCursor();

                            // Меню пользователя    
                            const std::string userMenu[]{
                                "Просмотр данных в табличной форме",
                                "Поиск данных",
                                "Фильтрация данных",
                                "Выполнить индивидуальное задание",
                                "Вернуться в главное меню",
                                "Выход из программы"
                            };
                            const int userMenu_Count = sizeof(userMenu) / sizeof(userMenu[0]);

                            int userChoice = main_showMenu(".../firstLevelMenu/registrationMenu/USER/userMenu", userMenu, userMenu_Count, 1);

                            switch (userChoice) {
                            case USER_VIEW_TABLE: {
                                system("cls");
                                AdTimeManager::printAdTimesTable(authSystem.adTimes);
                                system("pause");
                                break;
                            }
                            case USER_SEARCH: {
                                system("cls");
                                std::string searchQuery;
                                std::cout << "Введите название канала для поиска: ";
                                letter_filteredInput<std::string>(searchQuery, 1);

                                std::string lowerSearchQuery = toLower(searchQuery);
                                bool found = false;

                                system("cls");
                                std::vector<AdTime> searchResults;
                                for (int i = 0; i < authSystem.adTimes.size(); i++) {
                                    std::string lowerChannel = toLower(authSystem.adTimes[i].GetChannel());
                                    if (lowerChannel.find(lowerSearchQuery) != std::string::npos) {
                                        searchResults.push_back(authSystem.adTimes[i]);
                                        found = true;
                                    }
                                }

                                if (found) {
                                    AdTimeManager::printAdTimesTable(searchResults);
                                }
                                else {
                                    std::cout << "Ничего не найдено!\n";
                                }
                                system("pause");
                                break;
                            }
                            case USER_FILTER: {
                                std::vector<bool> selectedColumns(FILTER_COUNT, true);
                                bool selectionDone = false;

                                while (!selectionDone) {
                                    const std::string columnMenu[] = {
                                        std::string(selectedColumns[FILTER_CHANNEL] ? "[*]" : "[ ]") + " Канал",
                                        std::string(selectedColumns[FILTER_PROGRAM] ? "[*]" : "[ ]") + " Программа",
                                        std::string(selectedColumns[FILTER_DATE] ? "[*]" : "[ ]") + " Дата",
                                        std::string(selectedColumns[FILTER_TIME] ? "[*]" : "[ ]") + " Время",
                                        std::string(selectedColumns[FILTER_DURATION] ? "[*]" : "[ ]") + " Длительность",
                                        std::string(selectedColumns[FILTER_DAYS] ? "[*]" : "[ ]") + " Дни показа",
                                        std::string(selectedColumns[FILTER_SHOWS_PER_DAY] ? "[*]" : "[ ]") + " Показов в день",
                                        std::string(selectedColumns[FILTER_PRIME_TIME] ? "[*]" : "[ ]") + " Прайм-тайм",
                                        std::string(selectedColumns[FILTER_POSITION] ? "[*]" : "[ ]") + " Позиция",
                                        std::string(selectedColumns[FILTER_PRICE] ? "[*]" : "[ ]") + " Цена",
                                        "Подтвердить выбор",
                                        "Вернуться назад"
                                    };
                                    const int columnMenuCount = sizeof(columnMenu) / sizeof(columnMenu[0]);

                                    int choice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminFilter/", columnMenu, columnMenuCount, 1);

                                    if (choice >= 0 && choice < FILTER_COUNT - 2) {
                                        selectedColumns[choice] = !selectedColumns[choice];
                                    }
                                    else if (choice == FILTER_COUNT - 2) {
                                        if (std::count(selectedColumns.begin(), selectedColumns.end() - 2, true) > 0) {
                                            AdTimeManager::showSelectedColumns(authSystem.adTimes, selectedColumns);
                                            continue;
                                        }
                                    }
                                    else if (choice == FILTER_COUNT - 1) {
                                        selectionDone = true;
                                    }
                                }
                                break;
                            }
                            case USER_CUSTOM_TASK: {
                                AdTimeManager::showTop3ByAdDuration(authSystem.adTimes);
                                break;
                            }
                            case USER_GO_BACK: {
                                shouldReturnToFirstLevelMenu = true;
                                break;
                            }
                            case USER_EXIT: {
                                std::cout << "\nВы выбрали: " << userMenu[userChoice] << " ";
                                smoothWink();
                                return 0;
                            }
                            }
                        }
                    }
                    break;
                }
                case MENU_AUTH_GO_BACK: {
                    shouldReturnToFirstLevelMenu = true;
                    break;
                }
                }
            }
            break;
        }
        case MENU_ADMIN: {
            bool shouldReturnToFirstLevelMenu = false;
            if (checkAdminPassword()) {
                while (!shouldReturnToFirstLevelMenu) {
                    hideCursor();

                    // Меню регистрации/авторизации
                    const std::string registrationMenu[]{
                        "Зарегистрироваться",
                        "Войти в аккаунт",
                        "Вернуться назад"
                    };
                    const int registrationMenu_Count = sizeof(registrationMenu) / sizeof(registrationMenu[0]);

                    int registrationMenu_choice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/", registrationMenu, registrationMenu_Count);

                    switch (registrationMenu_choice) {
                    case MENU_AUTH_REGISTER: {
                        authSystem.registerUser();
                        break;
                    }
                    case MENU_AUTH_LOGIN: {
                        int userIndex = authSystem.loginUser();
                        if (userIndex != -1) {
                            std::cout << std::endl << "Добро пожаловать, " << authSystem.users[userIndex].GetFullname() << "!" << std::endl;
                            system("pause");

                            while (!shouldReturnToFirstLevelMenu) {
                                hideCursor();

                                // Меню админа
                                const std::string adminMenu[]{
                                     "Просмотр данных в табличной форме",
                                     "Добавить запись",
                                     "Редактировать запись",
                                     "Удалить запись",
                                     "Поиск данных",
                                     "Фильтрация данных",
                                     "Сортировка данных",
                                     "Выполнить индивидуальное задание",
                                     "Управление пользователями",
                                     "Вернуться в главное меню",
                                     "Выход из программы"
                                };
                                const int adminMenu_Count = sizeof(adminMenu) / sizeof(adminMenu[0]);

                                int adminChoice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminMenu/", adminMenu, adminMenu_Count, 1);

                                switch (adminChoice) {
                                case ADMIN_VIEW_TABLE: {
                                    system("cls");
                                    AdTimeManager::printAdTimesTable(authSystem.adTimes);
                                    system("pause");
                                    break;
                                }
                                case ADMIN_ADD_RECORD: {
                                    system("cls");
                                    std::cout << "Вы выбрали: " << adminMenu[adminChoice] << "\n";

                                    AdTime newAd;
                                    std::string input;
                                    int intInput;
                                    double doubleInput;

                                    std::cout << "Канал: ";
                                    letter_filteredInput<std::string>(input, 1);
                                    newAd.SetChannel(input);

                                    std::cout << "Программа: ";
                                    letter_filteredInput<std::string>(input, 1);
                                    newAd.SetProgram(input);

                                    bool dateValid = false;
                                    int day, month, year;
                                    std::string dateStr;
                                    while (!dateValid) {
                                        std::cout << "День: ";
                                        number_filteredInput<int>(day, 0, 1);

                                        std::cout << "Месяц: ";
                                        number_filteredInput<int>(month, 0, 1);

                                        bool yearValid = false, errValid = false;
                                        std::cout << "Год: ";
                                        while (!yearValid) {
                                            number_filteredInput<int>(year, 0, 1);
                                            if (year < 2026 && year > 2022) {
                                                yearValid = true;
                                            }
                                            else {
                                                errValid = true;
                                                yearValid = true;
                                            }
                                        }

                                        if (!isValidDate(day, month, year) || errValid) {
                                            std::cout << "Ошибка! Некорректная дата! Повторите ввод: \n";
                                            continue;
                                        }

                                        dateStr = (day < 10 ? "0" : "") + std::to_string(day) + "." + (month < 10 ? "0" : "") + std::to_string(month) + "." + std::to_string(year);
                                        newAd.SetDate(dateStr);
                                        dateValid = true;
                                    }

                                    bool startTimeValid = false;
                                    int startHour, startMinute;
                                    std::string startTimeStr;
                                    while (!startTimeValid) {
                                        bool hourValid = false;
                                        std::cout << "Час начала: ";
                                        while (!hourValid) {
                                            number_filteredInput<int>(startHour, 0, 1);
                                            if (startHour >= 0 && startHour < 24) {
                                                hourValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        bool minuteValid = false;
                                        std::cout << "Минуты начала: ";
                                        while (!minuteValid) {
                                            number_filteredInput<int>(startMinute, 0, 1);
                                            if (startMinute >= 0 && startMinute < 60) {
                                                minuteValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        startTimeStr = (startHour < 10 ? "0" : "") + std::to_string(startHour) + ":" +
                                            (startMinute < 10 ? "0" : "") + std::to_string(startMinute);
                                        newAd.SetStartTime(startTimeStr);
                                        startTimeValid = true;
                                    }

                                    bool endTimeValid = false;
                                    int endHour, endMinute;
                                    std::string endTimeStr;
                                    while (!endTimeValid) {
                                        bool hourValid = false;
                                        std::cout << "Час окончания: ";
                                        while (!hourValid) {
                                            number_filteredInput<int>(endHour, 0, 1);
                                            if (endHour >= 0 && endHour < 24) {
                                                hourValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        bool minuteValid = false;
                                        std::cout << "Минуты окончания: ";
                                        while (!minuteValid) {
                                            number_filteredInput<int>(endMinute, 0, 1);
                                            if (endMinute >= 0 && endMinute < 60) {
                                                minuteValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        if (endHour < startHour || (endHour == startHour && endMinute <= startMinute)) {
                                            std::cout << "Ошибка! Время окончания должно быть позже времени начала (" << startTimeStr << ")! Повторите ввод:\n";
                                            continue;
                                        }

                                        endTimeStr = (endHour < 10 ? "0" : "") + std::to_string(endHour) + ":" + (endMinute < 10 ? "0" : "") + std::to_string(endMinute);
                                        newAd.SetEndTime(endTimeStr);
                                        endTimeValid = true;
                                    }

                                    bool durationValid = false;
                                    std::cout << "Длительность (мин): ";
                                    while (!durationValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 270 && intInput != 0) {
                                            newAd.SetAdDuration(intInput);
                                            durationValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool showDaysValid = false;
                                    std::cout << "Дни показа: ";
                                    while (!showDaysValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 365 && intInput != 0) {
                                            newAd.SetDaysToShow(intInput);
                                            showDaysValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool showOfDaysValid = false;
                                    std::cout << "Показов в день: ";
                                    while (!showOfDaysValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 100 && intInput != 0) {
                                            newAd.SetShowsPerDay(intInput);
                                            showOfDaysValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool primeTimeValid = false;
                                    std::cout << "Прайм-тайм (1 - да, 0 - нет): ";
                                    while (!primeTimeValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        switch (intInput) {
                                        case 0:
                                            newAd.SetIsPrimeTime("Нет");
                                            primeTimeValid = true;
                                            break;
                                        case 1:
                                            newAd.SetIsPrimeTime("Да");
                                            primeTimeValid = true;
                                            break;
                                        default:
                                            std::cout << "Ошибка! Повторите ввод: ";
                                            break;
                                        }
                                    }

                                    bool positionValid = false;
                                    std::cout << "Позиция (1 - beginning, 2 - middle, 3 - end): ";
                                    while (!positionValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        switch (intInput) {
                                        case 1:
                                            newAd.SetAdPosition("beginning");
                                            positionValid = true;
                                            break;
                                        case 2:
                                            newAd.SetAdPosition("middle");
                                            positionValid = true;
                                            break;
                                        case 3:
                                            newAd.SetAdPosition("end");
                                            positionValid = true;
                                            break;
                                        default:
                                            std::cout << "Ошибка! Повторите ввод: ";
                                            break;
                                        }
                                    }

                                    bool cost = false;
                                    std::cout << "Цена: ";
                                    while (!cost) {
                                        number_filteredInput<double>(doubleInput, 0, 1);
                                        if (doubleInput != 0) {
                                            newAd.SetPrice(doubleInput);
                                            cost = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    authSystem.adTimes.push_back(newAd);
                                    authSystem.saveAdTimes();

                                    std::cout << "\nЗапись успешно добавлена!\n";
                                    system("pause");
                                    break;
                                }
                                case ADMIN_EDIT_RECORD: {
                                    system("cls");
                                    std::cout << "Вы выбрали: " << adminMenu[adminChoice] << "\n";
                                    AdTimeManager::printAdTimesTable(authSystem.adTimes);

                                    std::cout << "\nВведите номер записи для редактирования (0 - отмена): ";
                                    int recordNum;
                                    number_filteredInput<int>(recordNum, 0, 1);

                                    if (recordNum == 0) {
                                        break;
                                    }
                                    if (recordNum > authSystem.adTimes.size()) {
                                        std::cout << "Такой записи не существует!\n";
                                        system("pause");
                                        break;
                                    }

                                    AdTime& editAd = authSystem.adTimes[recordNum - 1];
                                    std::string input, input2;
                                    int intInput;
                                    double doubleInput;

                                    std::cout << "Канал: ";
                                    letter_filteredInput<std::string>(input, 1);
                                    editAd.SetChannel(input);

                                    std::cout << "Программа: ";
                                    letter_filteredInput<std::string>(input2, 1);
                                    editAd.SetProgram(input2);

                                    bool dateValid = false;
                                    int day, month, year;
                                    std::string dateStr;
                                    while (!dateValid) {
                                        std::cout << "День: ";
                                        number_filteredInput<int>(day, 0, 1);

                                        std::cout << "Месяц: ";
                                        number_filteredInput<int>(month, 0, 1);

                                        bool yearValid = false, errValid = false;
                                        std::cout << "Год: ";
                                        while (!yearValid) {
                                            number_filteredInput<int>(year, 0, 1);
                                            if (year < 2026 && year > 2022) {
                                                yearValid = true;
                                            }
                                            else {
                                                errValid = true;
                                                yearValid = true;
                                            }
                                        }

                                        if (!isValidDate(day, month, year) || errValid) {
                                            std::cout << "Ошибка! Некорректная дата! Повторите ввод: \n";
                                            continue;
                                        }

                                        dateStr = (day < 10 ? "0" : "") + std::to_string(day) + "." + (month < 10 ? "0" : "") + std::to_string(month) + "." + std::to_string(year);
                                        editAd.SetDate(dateStr);
                                        dateValid = true;
                                    }

                                    bool startTimeValid = false;
                                    int startHour, startMinute;
                                    std::string startTimeStr;
                                    while (!startTimeValid) {
                                        bool hourValid = false;
                                        std::cout << "Час начала: ";
                                        while (!hourValid) {
                                            number_filteredInput<int>(startHour, 0, 1);
                                            if (startHour >= 0 && startHour < 24) {
                                                hourValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        bool minuteValid = false;
                                        std::cout << "Минуты начала: ";
                                        while (!minuteValid) {
                                            number_filteredInput<int>(startMinute, 0, 1);
                                            if (startMinute >= 0 && startMinute < 60) {
                                                minuteValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        startTimeStr = (startHour < 10 ? "0" : "") + std::to_string(startHour) + ":" + (startMinute < 10 ? "0" : "") + std::to_string(startMinute);
                                        editAd.SetStartTime(startTimeStr);
                                        startTimeValid = true;
                                    }

                                    bool endTimeValid = false;
                                    int endHour, endMinute;
                                    std::string endTimeStr;
                                    while (!endTimeValid) {
                                        bool hourValid = false;
                                        std::cout << "Час окончания: ";
                                        while (!hourValid) {
                                            number_filteredInput<int>(endHour, 0, 1);
                                            if (endHour >= 0 && endHour < 24) {
                                                hourValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        bool minuteValid = false;
                                        std::cout << "Минуты окончания: ";
                                        while (!minuteValid) {
                                            number_filteredInput<int>(endMinute, 0, 1);
                                            if (endMinute >= 0 && endMinute < 60) {
                                                minuteValid = true;
                                            }
                                            else {
                                                std::cout << "Ошибка! Повторите ввод: ";
                                            }
                                        }

                                        if (endHour < startHour || (endHour == startHour && endMinute <= startMinute)) {
                                            std::cout << "Ошибка! Время окончания должно быть позже времени начала (" << startTimeStr << ")! Повторите ввод:\n";
                                            continue;
                                        }

                                        endTimeStr = (endHour < 10 ? "0" : "") + std::to_string(endHour) + ":" +
                                            (endMinute < 10 ? "0" : "") + std::to_string(endMinute);
                                        editAd.SetEndTime(endTimeStr);
                                        endTimeValid = true;
                                    }

                                    bool durationValid = false;
                                    std::cout << "Длительность (мин): ";
                                    while (!durationValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 270 && intInput != 0) {
                                            editAd.SetAdDuration(intInput);
                                            durationValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool showDaysValid = false;
                                    std::cout << "Дни показа: ";
                                    while (!showDaysValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 365 && intInput != 0) {
                                            editAd.SetDaysToShow(intInput);
                                            showDaysValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool showOfDaysValid = false;
                                    std::cout << "Показов в день: ";
                                    while (!showOfDaysValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        if (intInput < 100 && intInput != 0) {
                                            editAd.SetShowsPerDay(intInput);
                                            showOfDaysValid = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    bool primeTimeValid = false;
                                    std::cout << "Прайм-тайм (1 - да, 0 - нет): ";
                                    while (!primeTimeValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        switch (intInput) {
                                        case 0:
                                            editAd.SetIsPrimeTime("Нет");
                                            primeTimeValid = true;
                                            break;
                                        case 1:
                                            editAd.SetIsPrimeTime("Да");
                                            primeTimeValid = true;
                                            break;
                                        default:
                                            std::cout << "Ошибка! Повторите ввод: ";
                                            break;
                                        }
                                    }

                                    bool positionValid = false;
                                    std::cout << "Позиция (1 - beginning, 2 - middle, 3 - end): ";
                                    while (!positionValid) {
                                        number_filteredInput<int>(intInput, 0, 1);
                                        switch (intInput) {
                                        case 1:
                                            editAd.SetAdPosition("beginning");
                                            positionValid = true;
                                            break;
                                        case 2:
                                            editAd.SetAdPosition("middle");
                                            positionValid = true;
                                            break;
                                        case 3:
                                            editAd.SetAdPosition("end");
                                            positionValid = true;
                                            break;
                                        default:
                                            std::cout << "Ошибка! Повторите ввод: ";
                                            break;
                                        }
                                    }

                                    bool cost = false;
                                    std::cout << "Цена: ";
                                    while (!cost) {
                                        number_filteredInput<double>(doubleInput, 0, 1);
                                        if (doubleInput != 0) {
                                            editAd.SetPrice(doubleInput);
                                            cost = true;
                                        }
                                        else {
                                            std::cout << "Ошибка! Повторите ввод: ";
                                        }
                                    }

                                    authSystem.saveAdTimes();
                                    std::cout << "\nЗапись успешно изменена!\n";
                                    system("pause");
                                    break;
                                }
                                case ADMIN_DELETE_RECORD: {
                                    system("cls");
                                    std::cout << "Вы выбрали: " << adminMenu[adminChoice] << "\n";
                                    AdTimeManager::printAdTimesTable(authSystem.adTimes);

                                    std::cout << "\nВведите номер записи для удаления (0 - отмена): ";
                                    int recordNum;
                                    number_filteredInput<int>(recordNum, 0, 1);

                                    if (recordNum == 0) {
                                        std::cout << "Удаление отменено!\n";
                                        system("pause");
                                        break;
                                    }
                                    if (recordNum > authSystem.adTimes.size()) {
                                        std::cout << "Такой записи не существует!\n";
                                        system("pause");
                                        break;
                                    }

                                    authSystem.adTimes.erase(authSystem.adTimes.begin() + recordNum - 1);
                                    authSystem.saveAdTimes();
                                    std::cout << "Запись успешно удалена!\n";

                                    system("pause");
                                    break;
                                }
                                case ADMIN_SEARCH: {
                                    system("cls");
                                    std::string searchQuery;
                                    std::cout << "Введите название канала для поиска: ";
                                    letter_filteredInput<std::string>(searchQuery, 1);

                                    std::string lowerSearchQuery = toLower(searchQuery);
                                    bool found = false;

                                    system("cls");
                                    std::vector<AdTime> searchResults;
                                    for (int i = 0; i < authSystem.adTimes.size(); i++) {
                                        std::string lowerChannel = toLower(authSystem.adTimes[i].GetChannel());
                                        if (lowerChannel.find(lowerSearchQuery) != std::string::npos) {
                                            searchResults.push_back(authSystem.adTimes[i]);
                                            found = true;
                                        }
                                    }

                                    if (found) {
                                        AdTimeManager::printAdTimesTable(searchResults);
                                    }
                                    else {
                                        std::cout << "Ничего не найдено!\n";
                                    }
                                    system("pause");
                                    break;
                                }
                                case ADMIN_FILTER: {
                                    std::vector<bool> selectedColumns(FILTER_COUNT, true);
                                    bool selectionDone = false;

                                    while (!selectionDone) {
                                        const std::string columnMenu[] = {
                                            std::string(selectedColumns[FILTER_CHANNEL] ? "[*]" : "[ ]") + " Канал",
                                            std::string(selectedColumns[FILTER_PROGRAM] ? "[*]" : "[ ]") + " Программа",
                                            std::string(selectedColumns[FILTER_DATE] ? "[*]" : "[ ]") + " Дата",
                                            std::string(selectedColumns[FILTER_TIME] ? "[*]" : "[ ]") + " Время",
                                            std::string(selectedColumns[FILTER_DURATION] ? "[*]" : "[ ]") + " Длительность",
                                            std::string(selectedColumns[FILTER_DAYS] ? "[*]" : "[ ]") + " Дни показа",
                                            std::string(selectedColumns[FILTER_SHOWS_PER_DAY] ? "[*]" : "[ ]") + " Показов в день",
                                            std::string(selectedColumns[FILTER_PRIME_TIME] ? "[*]" : "[ ]") + " Прайм-тайм",
                                            std::string(selectedColumns[FILTER_POSITION] ? "[*]" : "[ ]") + " Позиция",
                                            std::string(selectedColumns[FILTER_PRICE] ? "[*]" : "[ ]") + " Цена",
                                            "Подтвердить выбор",
                                            "Вернуться назад"
                                        };
                                        const int columnMenuCount = sizeof(columnMenu) / sizeof(columnMenu[0]);

                                        int choice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminFilter/", columnMenu, columnMenuCount, 1);

                                        if (choice >= 0 && choice < FILTER_COUNT - 2) {
                                            selectedColumns[choice] = !selectedColumns[choice];
                                        }
                                        else if (choice == FILTER_COUNT - 2) {
                                            if (std::count(selectedColumns.begin(), selectedColumns.end() - 2, true) > 0) {
                                                AdTimeManager::showSelectedColumns(authSystem.adTimes, selectedColumns);
                                                continue;
                                            }
                                        }
                                        else if (choice == FILTER_COUNT - 1) {
                                            selectionDone = true;
                                        }
                                    }
                                    break;
                                }
                                case ADMIN_SORT: {
                                    std::vector<AdTime> sortedAdTimes = authSystem.adTimes;

                                    const std::string sortMenu[] = {
                                        "Сортировать по каналу",
                                        "Сортировать по программе",
                                        "Сортировать по дате",
                                        "Сортировать по времени",
                                        "Сортировать по длительности",
                                        "Сортировать по дням показа",
                                        "Сортировать по показам в день",
                                        "Сортировать по прайм-тайму",
                                        "Сортировать по позиции",
                                        "Сортировать по цене",
                                        "Вернуться назад"
                                    };
                                    const int sortMenuCount = sizeof(sortMenu) / sizeof(sortMenu[0]);

                                    int sortChoice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminMenu/sortMenu/",
                                        sortMenu, sortMenuCount, 1);

                                    if (sortChoice >= 0 && sortChoice < sortMenuCount - 1) {
                                        AdTimeManager::bubbleSort(sortedAdTimes, sortChoice);

                                        system("cls");
                                        AdTimeManager::printAdTimesTable(sortedAdTimes);
                                        system("pause");
                                    }
                                    break;
                                }
                                case ADMIN_CUSTOM_TASK: {
                                    AdTimeManager::showTop3ByAdDuration(authSystem.adTimes);
                                    break;
                                }
                                case ADMIN_MANAGE_USERS: {
                                    bool shouldReturnToAdminMenu = false;
                                    while (!shouldReturnToAdminMenu) {
                                        hideCursor();

                                        const std::string userMgmtMenu[]{
                                            "Просмотр всех пользователей",
                                            "Заблокировать пользователя",
                                            "Разблокировать пользователя",
                                            "Удалить пользователя",
                                            "Вернуться назад"
                                        };
                                        const int userMgmtMenu_Count = sizeof(userMgmtMenu) / sizeof(userMgmtMenu[0]);

                                        int userMgmtChoice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminMenu/userManagement/", userMgmtMenu, userMgmtMenu_Count, 1);

                                        switch (userMgmtChoice) {
                                        case USER_MGMT_VIEW_ALL: {
                                            std::cout << "----------------------------------------------------\n";
                                            std::cout << "| №    | Полное имя      | Логин        | Статус   |\n";
                                            std::cout << "----------------------------------------------------\n";

                                            int index = 1;
                                            for (int i = 0; i < authSystem.users.size(); i++) {
                                                std::cout << "| " << std::setw(4) << std::left << index++ << " | ";
                                                std::cout << std::setw(15) << std::left << authSystem.users[i].GetFullname() << " | ";
                                                std::cout << std::setw(12) << std::left << authSystem.users[i].GetUsername() << " | ";
                                                std::cout << std::setw(8) << std::left << (authSystem.users[i].GetIsBlocked() ? "Заблок." : "Активен") << " |\n";
                                            }
                                            std::cout << "-----------------------------------------------------\n";
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_BLOCK: {
                                            std::cout << "Вы выбрали: " << userMgmtMenu[userMgmtChoice] << "\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            bool found = false;
                                            for (int i = 0; i < authSystem.users.size(); i++) {
                                                if (authSystem.users[i].GetUsername() == username) {
                                                    found = true;

                                                    if (authSystem.users[i].GetIsBlocked()) {
                                                        std::cout << "Пользователь " << username << " уже заблокирован!\n";
                                                    }
                                                    else {
                                                        authSystem.users[i].SetIsBlocked(true);
                                                        authSystem.saveUsers();
                                                        std::cout << "Пользователь " << username << " успешно будет заблокирован по завершению сессии!\n";
                                                    }
                                                    break;
                                                }
                                            }

                                            if (!found) {
                                                std::cout << "Пользователь с таким логином не найден!\n";
                                            }
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_UNBLOCK: {
                                            std::cout << "Вы выбрали: " << userMgmtMenu[userMgmtChoice] << "\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            bool found = false;
                                            for (int i = 0; i < authSystem.users.size(); i++) {
                                                if (authSystem.users[i].GetUsername() == username) {
                                                    found = true;

                                                    if (!authSystem.users[i].GetIsBlocked()) {
                                                        std::cout << "Пользователь " << username << " не был заблокирован!\n";
                                                    }
                                                    else {
                                                        authSystem.users[i].SetIsBlocked(false);
                                                        authSystem.saveUsers();
                                                        std::cout << "Пользователь " << username << " успешно разблокирован!\n";
                                                    }
                                                    break;
                                                }
                                            }

                                            if (!found) {
                                                std::cout << "Пользователь с таким логином не найден!\n";
                                            }
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_DELETE: {
                                            std::cout << "Вы выбрали: " << userMgmtMenu[userMgmtChoice] << "\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            bool found = false;
                                            for (int i = 0; i < authSystem.users.size(); i++) {
                                                if (authSystem.users[i].GetUsername() == username) {
                                                    authSystem.users.erase(authSystem.users.begin() + i);
                                                    authSystem.saveUsers();
                                                    std::cout << "Пользователь " << username << " будет успешно удален по завершению сессии!\n";
                                                    found = true;
                                                    break;
                                                }
                                            }

                                            if (!found) {
                                                std::cout << "Пользователь с таким логином не найден!\n";
                                            }
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_GO_BACK: {
                                            shouldReturnToAdminMenu = true;
                                            break;
                                        }
                                        }
                                    }
                                    break;
                                }
                                case ADMIN_GO_BACK: {
                                    shouldReturnToFirstLevelMenu = true;
                                    break;
                                }
                                case ADMIN_EXIT: {
                                    std::cout << "\nВы выбрали: " << adminMenu[adminChoice] << " ";
                                    smoothWink();
                                    return 0;
                                }
                                default:
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case MENU_AUTH_GO_BACK: {
                        shouldReturnToFirstLevelMenu = true;
                        break;
                    }
                    }
                }
            }
            else {
                std::cout << "Неверный пароль, доступ запрещен!" << std::endl;
                system("pause");
            }
            break;
        }
        case EXIT_PROGRAM: {
            std::cout << "\nВы выбрали: " << firstLevelMenu[firstLevelMenu_choice] << " ";
            smoothWink();
            return 0;
        }
        }
    }
    return 0;
}