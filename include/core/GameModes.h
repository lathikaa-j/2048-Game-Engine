#ifndef GAME_MODES_H
#define GAME_MODES_H

#include "Board.h"

// Assuming you have a base class, or just a standalone TargetMode manager
class TargetMode {
private:
    int targetTile;

public:
    // Constructor now requires the user's chosen target
    TargetMode(int target);

    // Function to check if the user has reached their custom goal
    bool isTargetReached(const Board& board) const;
    
    int getTarget() const;
};

#endif // GAME_MODES_H