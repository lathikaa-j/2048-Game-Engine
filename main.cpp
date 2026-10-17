#include "core/Game.h"
#include "exceptions/GameException.h"
#include "ui/Renderer.h"
#include "ui/InputHandler.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Renderer renderer(argc > 1 ? argv[1] : "assets/fonts/2048.ttf");
        Game game;
        InputHandler input;
        sf::RenderWindow window(sf::VideoMode(Renderer::WINDOW_WIDTH, Renderer::WINDOW_HEIGHT),
            "2048 Game Engine", sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        bool focused = window.hasFocus();
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { window.close(); }
                if (event.type == sf::Event::LostFocus) { focused = false; }
                if (event.type == sf::Event::GainedFocus) { focused = true; }
                if (input.handleEvent(event, game, focused) == InputHandler::Result::CloseRequested) {
                    window.close();
                }
                if (focused && event.type == sf::Event::MouseButtonPressed
                    && renderer.restartClicked(event,
                        window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}), game)) {
                    game.newGame();
                }
            }
            if (!window.isOpen()) { break; }
            window.clear(Renderer::backgroundColor());
            renderer.render(window, game);
            window.display();
        }
    } catch (const GameException& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
