#include "console_inactivity_timer.h"

void inactivity_timer(int seconds) {
    HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);

    auto last_active = std::chrono::steady_clock::now();

    while (true) {
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                last_active = std::chrono::steady_clock::now();
                break;
            }
        }

        if (std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - last_active).count() >= seconds) {
            system("cls");
            SetConsoleTextAttribute(consoleColor, 4);
            std::cout << "\n=========================================\n";
            std::cout <<   "= Программа закрыта из-за неактивности! =";
            std::cout << "\n=========================================\n";
            SetConsoleTextAttribute(consoleColor, 7);
            Sleep(3000);
            exit(0);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}