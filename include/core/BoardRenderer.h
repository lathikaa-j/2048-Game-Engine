#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

#include <vector>

class BoardRenderer {
public:
    void drawBoard(const std::vector<std::vector<int>>& board) const;
};

#endif