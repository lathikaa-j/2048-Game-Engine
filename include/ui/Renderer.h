#pragma once
#include "core/Game.h"
#include <SFML/Graphics.hpp>
#include <string>

#if SFML_VERSION_MAJOR != 2 || SFML_VERSION_MINOR < 6
#error "Renderer requires SFML 2.6.x; SFML 3 uses a different API"
#endif

// Presentation only: reads the authoritative game through const references.
class Renderer {
private:
    sf::Font font;
    static constexpr float BOARD_SIZE = 500.f;
    static constexpr float BOARD_TOP = 195.f;
    static constexpr float CELL_GAP = 12.f;
    static constexpr float TITLE_Y = 65.f;
    static constexpr float SCORE_Y = 133.f;
    static constexpr float STATE_Y = 720.f;
    void drawBoard(sf::RenderWindow& window, const Board& board) const;
    void drawTile(sf::RenderWindow& window, int value, sf::Vector2f position, float size) const;
    void drawScore(sf::RenderWindow& window, const Game& game) const;
    void drawCenteredText(sf::RenderWindow& window, const std::string& label,
        unsigned int size, sf::Vector2f center, sf::Color color) const;
    sf::Color getTileColor(int value) const;
    unsigned int getTileFontSize(int value) const;
public:
    static constexpr unsigned int WINDOW_WIDTH = 600;
    static constexpr unsigned int WINDOW_HEIGHT = 750;
    explicit Renderer(const std::string& fontPath);
    void render(sf::RenderWindow& window, const Game& game) const;
    static sf::Color backgroundColor();
};
