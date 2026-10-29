#include "core/DynamicBoard.h"
#include <iostream>

DynamicBoard::DynamicBoard(int initialSize) {
    currentSize = initialSize;
    nextExpansionTarget = 128; // First milestone to trigger 4x4 expansion

    // Initialize the 2D vector with empty tiles
    grid.resize(currentSize, std::vector<Tile*>(currentSize, nullptr));
    for (int y = 0; y < currentSize; ++y) {
        for (int x = 0; x < currentSize; ++x) {
            grid[y][x] = new Tile(0);
        }
    }
}

DynamicBoard::~DynamicBoard() {
    for (int y = 0; y < currentSize; ++y) {
        for (int x = 0; x < currentSize; ++x) {
            delete grid[y][x];
        }
    }
}

int DynamicBoard::getSize() const {
    return currentSize;
}

void DynamicBoard::checkAndExpand() {
    bool milestoneReached = false;
    
    // Scan the board for the target tile
    for (int y = 0; y < currentSize; ++y) {
        for (int x = 0; x < currentSize; ++x) {
            if (grid[y][x]->getValue() >= nextExpansionTarget) {
                milestoneReached = true;
                break;
            }
        }
    }

    if (milestoneReached) {
        expandBoard();
        // Scale the next target drastically (e.g., 128 -> 1024 -> 8192)
        nextExpansionTarget *= 8; 
    }
}

void DynamicBoard::expandBoard() {
    int newSize = currentSize + 1;
    
    // Create a new, larger grid
    std::vector<std::vector<Tile*>> newGrid(newSize, std::vector<Tile*>(newSize, nullptr));

    // Populate the entire new grid with empty tiles first
    for (int y = 0; y < newSize; ++y) {
        for (int x = 0; x < newSize; ++x) {
            newGrid[y][x] = new Tile(0);
        }
    }

    // Transfer existing tiles into the top-left corner of the new grid
    for (int y = 0; y < currentSize; ++y) {
        for (int x = 0; x < currentSize; ++x) {
            delete newGrid[y][x]; // Prevent memory leak of the empty tile we just made
            newGrid[y][x] = grid[y][x]; // Transfer the pointer
        }
    }

    // Swap the active grid to the newly allocated one
    grid = newGrid;
    currentSize = newSize;
}