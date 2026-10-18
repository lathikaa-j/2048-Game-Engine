#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

enum class Command {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    QUIT,
    INVALID
};

class InputHandler {
public:
    Command getGameInput() const;
};

#endif