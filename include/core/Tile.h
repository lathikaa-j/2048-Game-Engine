#pragma once

// Values are expected to be zero or positive powers of two that fit in int.
class Tile {
private:
    int value;

public:
    explicit Tile(int value = 0);

    int getValue() const;
    void setValue(int value);
    bool isEmpty() const;
};
