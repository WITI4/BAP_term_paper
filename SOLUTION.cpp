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

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    std::thread timer(inactivity_timer, 100);
    timer.detach();

    enum FirstLevelMenu { MENU_USER, MENU_ADMIN, EXIT_PROGRAM };
    enum RegistrationMenu { MENU_AUTH_REGISTER, MENU_AUTH_LOGIN, MENU_AUTH_GO_BACK };
    enum UserMenu { USER_VIEW_TABLE, USER_SEARCH, USER_FILTER, USER_CUSTOM_TASK, USER_GO_BACK, USER_EXIT };
    enum AdminMenu { ADMIN_VIEW_TABLE, ADMIN_ADD_RECORD, ADMIN_EDIT_RECORD, ADMIN_DELETE_RECORD, ADMIN_SEARCH, ADMIN_FILTER, ADMIN_SORT, ADMIN_CUSTOM_TASK, ADMIN_MANAGE_USERS, ADMIN_GO_BACK, ADMIN_EXIT };

    AuthSystem authSystem;
    User* currentUser = nullptr;

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
                    currentUser = authSystem.loginUser();
                    if (currentUser != nullptr) {
                        std::cout << std::endl << "Добро пожаловать, " << currentUser->GetFullname() << "!" << std::endl;
                        system("pause");

                        // Меню пользователя    
                        while (!shouldReturnToFirstLevelMenu) {
                            hideCursor();

                            const std::string userMenu[]{
                                "Просмотр данных в табличной форме",
                                "Поиск данных",
                                "Фильтрация данных",
                                "Выполнить индивидуальное задание",
                                "Вернуться в главное меню",
                                "Выход из программы"
                            };
                            const int userMenu_Count = sizeof(userMenu) / sizeof(userMenu[0]);

                            int userChoice = main_showMenu(".../firstLevelMenu/registrationMenu/userMenu", userMenu, userMenu_Count, 1);

                            switch (userChoice) {
                            case USER_VIEW_TABLE: {
                                system("pause");
                                break;
                            }
                            case USER_SEARCH: {
                                system("pause");
                                break;
                            }
                            case USER_FILTER: {
                                system("pause");
                                break;
                            }
                            case USER_CUSTOM_TASK: {
                                system("pause");
                                break;
                            }
                            case USER_GO_BACK: {
                                shouldReturnToFirstLevelMenu = true;
                                break;
                            }
                            case USER_EXIT: {
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
                        currentUser = authSystem.loginUser();
                        if (currentUser != nullptr) {
                            std::cout << std::endl << "Добро пожаловать, " << currentUser->GetFullname() << "!" << std::endl;
                            system("pause");

                            // Меню админа    
                            while (!shouldReturnToFirstLevelMenu) {
                                hideCursor();

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

                                int adminChoice = main_showMenu(".../firstLevelMenu/adminMenu/", adminMenu, adminMenu_Count, 1);

                                switch (adminChoice) {
                                case ADMIN_VIEW_TABLE: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_ADD_RECORD: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_EDIT_RECORD: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_DELETE_RECORD: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_SEARCH: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_FILTER: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_SORT: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_CUSTOM_TASK: {
                                    system("pause");
                                    break;
                                }
                                case ADMIN_MANAGE_USERS: {
                                    break;
                                }
                                case ADMIN_GO_BACK: {
                                    shouldReturnToFirstLevelMenu = true;
                                    break;
                                }
                                case ADMIN_EXIT: {
                                    std::cout << "\nВы выбрали: " << adminMenu[adminChoice] << std::endl;
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
                std::cout << "Неверный пароль! Доступ запрещен." << std::endl;
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