#include "../../include/Core/CliMenu.h"
#include <iostream>

void CLIMenu::displayMainMenu() const {
    std::cout << "\n=========================\n";
    std::cout << "    2048 GAME ENGINE     \n";
    std::cout << "=========================\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. View High Scores\n";
    std::cout << "4. View Statistics\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int CLIMenu::getUserChoice() const {
    int choice;
    std::cin >> choice;
    return choice;
}