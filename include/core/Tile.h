#ifndef TILE_H
#define TILE_H

class Tile {
protected:
    int value; // 0 = empty, 2/4/8 = standard, -1 = obstacle

public:
    // Constructor
    Tile(int val = 0);

    // Virtual destructor is required for safe polymorphic deletion
    virtual ~Tile() = default;

    // Getters and Setters
    int getValue() const;
    virtual void setValue(int val);
    virtual bool isEmpty() const;

    // --- Core OOP Feature: Polymorphic Game Logic ---
    // These methods can be overridden by specialized tiles
    virtual bool canMergeWith(const Tile* other) const;
    virtual void mergeWith(Tile* other);

    // Determines if the tile can slide across the board
    virtual bool isMovable() const;
};

// Derived Obstacle Class
class ObstacleTile : public Tile {
public:
    ObstacleTile();

    // Overriding base class behaviors
    bool canMergeWith(const Tile* other) const override;
    void mergeWith(Tile* other) override;
    bool isMovable() const override;
};

#endif // TILE_H