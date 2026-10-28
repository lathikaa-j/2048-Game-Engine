#ifndef TEMPLATE_COMPONENT_H
#define TEMPLATE_COMPONENT_H
#include <vector>
#include <stdexcept>
template <typename T>
class TemplateComponent {
private:
    std::vector<std::vector<T>> grid;
    size_t rows;
    size_t cols;
public:
    // Constructor initializes the generic grid with a specific size and default value
    TemplateComponent(size_t r, size_t c, T defaultValue) 
        : rows(r), cols(c), grid(r, std::vector<T>(c, defaultValue)) {}
    // Get a value from the generic board
    T getValue(size_t r, size_t c) const {
        if (r >= rows || c >= cols) {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return grid[r][c];
    }
    // Set a value on the generic board
    void setValue(size_t r, size_t c, T value) {
        if (r >= rows || c >= cols) {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        grid[r][c] = value;
    }
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Clear the entire generic storage back to a default value
    void reset(T defaultValue) {
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                grid[r][c] = defaultValue;
            }
        }
    }
};

#endif