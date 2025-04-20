#include "smoothWink.h"

void smoothWink() {
    const std::string emotions[3] = { "(^_^)", "(^_^)", "(^_~)" };

    for (int frame = 0; frame < 3; frame++) {
        std::cout << emotions[frame];
        std::cout.flush();
        Sleep(frame == 1 ? 300 : 600);

        for (size_t i = 0; i < emotions[frame].length(); i++) {
            std::cout << "\b";
        }
    }

    std::cout << emotions[0];
    Sleep(1000);

    for (size_t i = 0; i < emotions[0].length(); i++) {
        std::cout << "\b \b";
    }

    std::cout << "(^_^)\n\n";
}