#include "ui/Application.h"
#include <iostream>
int main(int argc, char* argv[]) {
    try {
        Application application(argc > 1 ? argv[1] : "assets/fonts/2048.ttf");
        return application.run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
