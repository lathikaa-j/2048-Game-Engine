#include "core/CliMenu.h"
#include <iostream>
#include <limits>

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
    // Loop until we successfully extract an integer
    while (!(std::cin >> choice)) {
        // Clear the fail state
        std::cin.clear();
        // Ignore the rest of the garbage on the line
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    return choice;
}