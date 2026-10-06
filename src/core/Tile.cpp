#include "core/Tile.h"

Tile::Tile(int value) : value(value) {}

int Tile::getValue() const {
    return value;
}

void Tile::setValue(int value) {
    this->value = value;
}

bool Tile::isEmpty() const {
    return value == 0;
}
