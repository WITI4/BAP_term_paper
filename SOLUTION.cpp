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
    enum UserManagementMenu { USER_MGMT_VIEW_ALL, USER_MGMT_BLOCK, USER_MGMT_UNBLOCK, USER_MGMT_DELETE, USER_MGMT_GO_BACK };
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

                            int userChoice = main_showMenu(".../firstLevelMenu/registrationMenu/USER/userMenu", userMenu, userMenu_Count, 1);

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

                                int adminChoice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminMenu/", adminMenu, adminMenu_Count, 1);

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

                                        int userMgmtChoice = main_showMenu(".../firstLevelMenu/registrationMenu/ADMIN/adminMenu/userManagement/", userMgmtMenu, userMgmtMenu_Count);

                                        switch (userMgmtChoice) {
                                        case USER_MGMT_VIEW_ALL: {
                                            system("cls");
                                            std::cout << "Список всех пользователей:\n\n";
                                            std::cout << "------------------------------------------------\n";
                                            std::cout << "| №  | Полное имя      | Логин      | Статус   |\n";
                                            std::cout << "------------------------------------------------\n";

                                            int index = 1;
                                            for (const auto& user : authSystem.users) {
                                                std::cout << "| " << index++ << "  | ";
                                                std::cout.width(15);
                                                std::cout << std::left << user.GetFullname() << " | ";
                                                std::cout.width(10);
                                                std::cout << std::left << user.GetUsername() << " | ";
                                                std::cout.width(8);
                                                std::cout << std::left << (user.GetIsBlocked() ? "Заблок." : "Активен") << " |\n";
                                            }
                                            std::cout << "------------------------------------------------\n";
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_BLOCK: {
                                            system("cls");
                                            std::cout << "Заблокировать пользователя:\n\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            bool found = false;
                                            for (auto& user : authSystem.users) {
                                                if (user.GetUsername() == username) {
                                                    user.SetIsBlocked(true);
                                                    authSystem.saveUsers();
                                                    std::cout << "\nПользователь " << username << " успешно заблокирован.\n";
                                                    found = true;
                                                    break;
                                                }
                                            }

                                            if (!found) {
                                                std::cout << "\nПользователь с таким логином не найден.\n";
                                            }
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_UNBLOCK: {
                                            system("cls");
                                            std::cout << "Разблокировать пользователя:\n\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            bool found = false;
                                            for (auto& user : authSystem.users) {
                                                if (user.GetUsername() == username) {
                                                    user.SetIsBlocked(false);
                                                    authSystem.saveUsers();
                                                    std::cout << "\nПользователь " << username << " успешно разблокирован.\n";
                                                    found = true;
                                                    break;
                                                }
                                            }

                                            if (!found) {
                                                std::cout << "\nПользователь с таким логином не найден.\n";
                                            }
                                            system("pause");
                                            break;
                                        }
                                        case USER_MGMT_DELETE: {
                                            system("cls");
                                            std::cout << "Удалить пользователя:\n\n";
                                            std::cout << "Введите логин пользователя: ";
                                            std::string username;
                                            letter_filteredInput<std::string>(username);

                                            auto it = std::remove_if(authSystem.users.begin(), authSystem.users.end(),
                                                [&username](const User& user) {
                                                    return user.GetUsername() == username;
                                                });

                                            if (it != authSystem.users.end()) {
                                                authSystem.users.erase(it, authSystem.users.end());
                                                authSystem.saveUsers();
                                                std::cout << "\nПользователь " << username << " успешно удален.\n";
                                            }
                                            else {
                                                std::cout << "\nПользователь с таким логином не найден.\n";
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