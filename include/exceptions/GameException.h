#pragma once

#include <exception>
#include <string>

class GameException : public std::exception {
private:
    std::string message;

public:
    explicit GameException(const std::string& message) : message(message) {}
    const char* what() const noexcept override { return message.c_str(); }
};
