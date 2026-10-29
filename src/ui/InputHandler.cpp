#include "ui/InputHandler.h"

InputHandler::Result InputHandler::handleEvent(const sf::Event& event, Game& game,
                                              bool gameplayOwnsFocus) const {
    if (!gameplayOwnsFocus || event.type != sf::Event::KeyPressed
        || game.getState() == GameState::Paused) { return Result::Ignored; }
    switch (event.key.code) {
        case sf::Keyboard::Escape: return Result::CloseRequested;
        case sf::Keyboard::R: game.newGame(); break;
        case sf::Keyboard::U: game.undo(); break;
        case sf::Keyboard::Up: case sf::Keyboard::W: game.move(Direction::Up); break;
        case sf::Keyboard::Down: case sf::Keyboard::S: game.move(Direction::Down); break;
        case sf::Keyboard::Left: case sf::Keyboard::A: game.move(Direction::Left); break;
        case sf::Keyboard::Right: case sf::Keyboard::D: game.move(Direction::Right); break;
        default: return Result::Ignored;
        case sf::Keyboard::Escape: return Result::CloseRequested;
    }
    return Result::Handled;
}
