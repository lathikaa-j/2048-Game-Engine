#ifndef DYNAMIC_BOARD_H
#define DYNAMIC_BOARD_H

#include <vector>
#include "Tile.h"

class DynamicBoard {
private:
    // Using a 2D vector allows for runtime resizing
    std::vector<std::vector<Tile*>> grid;
    int currentSize;
    int nextExpansionTarget;

    // Encapsulated helper function for the expansion logic
    void expandBoard();

public:
    // Constructor defaults to a 3x3 grid
    DynamicBoard(int initialSize = 3);
    
    // Destructor to clean up raw pointers
    ~DynamicBoard();

    int getSize() const;
    void spawnRandomTile();
    
    // Call this after every successful move to verify if the board should grow
    void checkAndExpand();

    // Example move function signature (others omitted for brevity)
    void moveLeft();
};

#endif // DYNAMIC_BOARD_H