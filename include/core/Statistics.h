#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {
private:
    int totalMoves = 0;
    int bestTile = 0;
    int gamesPlayed = 0;

public:
    void incrementMoves();
    void updateBestTile(int tileValue);
    void incrementGamesPlayed();
    
    int getMoves() const;
    int getBestTile() const;
    int getGamesPlayed() const;
};

#endif