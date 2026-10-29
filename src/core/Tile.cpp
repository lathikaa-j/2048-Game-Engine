#include "core/Tile.h"
Tile::Tile(int val) : value(val) {}

int Tile::getValue() const { 
    return value; 
}

void Tile::setValue(int val) { 
    value = val; 
}

bool Tile::isEmpty() const { 
    return value == 0; 
}

bool Tile::canMergeWith(const Tile* other) const {
    // Cannot merge if the other tile is null, or if either tile is empty
    if (!other || this->isEmpty() || other->isEmpty()) {
        return false;
    }
    // Standard tiles merge if their values match
    return this->value == other->getValue();
}

void Tile::mergeWith(Tile* other) {
    if (canMergeWith(other)) {
        this->value *= 2;      // Double the current tile's value
        other->setValue(0);    // Clear the consumed tile
    }
}

bool Tile::isMovable() const {
    return true; // Standard tiles can always slide
}


// Initialize the base Tile with -1 to represent an obstacle state
ObstacleTile::ObstacleTile() : Tile(-1) {}

bool ObstacleTile::canMergeWith(const Tile* other) const {
    // Obstacles reject all merge attempts, even with other obstacles
    return false; 
}

void ObstacleTile::mergeWith(Tile* other) {
    // Intentionally left blank. An obstacle does nothing when merged.
}

bool ObstacleTile::isMovable() const {
    // Returns false so the Board.cpp shifting logic knows to skip moving this tile
    return false; 
}