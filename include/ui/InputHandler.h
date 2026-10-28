#pragma once
#include "core/Game.h"
#include <SFML/Window/Event.hpp>

// Screen owners pass false while a menu/modal or lost window focus owns input.
class InputHandler {
public:
    enum class Result { Ignored, Handled, CloseRequested };
    Result handleEvent(const sf::Event& event, Game& game,
                       bool gameplayOwnsFocus = true) const;
};
