#include "../../include/core/InputHandler.h"
#include <iostream>

Command InputHandler::getGameInput() const {
    char input;
    std::cout << "Enter move (W=Up, S=Down, A=Left, D=Right, Q=Quit): ";
    std::cin >> input;
    
    switch(input) {
        case 'w': case 'W': return Command::UP;
        case 's': case 'S': return Command::DOWN;
        case 'a': case 'A': return Command::LEFT;
        case 'd': case 'D': return Command::RIGHT;
        case 'q': case 'Q': return Command::QUIT;
        default: return Command::INVALID;
    }
}