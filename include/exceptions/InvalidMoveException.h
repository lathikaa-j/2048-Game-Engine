#pragma once

#include "exceptions/GameException.h"

class InvalidMoveException : public GameException {
public:
    explicit InvalidMoveException(const std::string& message) : GameException(message) {}
};
