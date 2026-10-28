#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class History {
private:
    std::vector<T> states;

public:
    void push(const T& state) { states.push_back(state); }

    T pop() {
        T state = top();
        states.pop_back();
        return state;
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("History is empty");
        }
        return states.back();
    }

    bool empty() const { return states.empty(); }
    std::size_t size() const { return states.size(); }
    void clear() { states.clear(); }
};
