#pragma once
#include "core/Game.h"
#include "ui/Button.h"
#include <SFML/Graphics.hpp>
#include <string>

#if SFML_VERSION_MAJOR != 2 || SFML_VERSION_MINOR < 6
#error "Renderer requires SFML 2.6.x; SFML 3 uses a different API"
#endif

// Presentation only: reads the authoritative game through const references.
class Renderer {
private:
    sf::Font font;
    Button newGameButton{{365.f, 111.f, 185.f, 48.f}, "New Game"};
    Button overlayButton{{200.f, 495.f, 200.f, 50.f}, "New Game"};
    void drawOverlay(sf::RenderWindow& window, const Game& game) const;
    static constexpr float BOARD_SIZE = 500.f;
    static constexpr float BOARD_TOP = 195.f;
    static constexpr float CELL_GAP = 12.f;
    static constexpr float TITLE_Y = 65.f;
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
    bool restartClicked(const sf::Event& event, sf::Vector2f point, const Game& game) const;
    static sf::Color backgroundColor();
};
