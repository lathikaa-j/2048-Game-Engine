#include "../../include/Core/BoardRenderer.h"
#include <iostream>
#include <iomanip>

void BoardRenderer::drawBoard(const std::vector<std::vector<int>>& board) const {
    std::cout << "\n-----------------------------\n";
    for (size_t r = 0; r < board.size(); ++r) {
        for (size_t c = 0; c < board[r].size(); ++c) {
            if (board[r][c] == 0) {
                std::cout << std::setw(6) << "."; 
            } else {
                std::cout << std::setw(6) << board[r][c];
            }
        }
        std::cout << "\n\n";
    }
    std::cout << "-----------------------------\n";
}