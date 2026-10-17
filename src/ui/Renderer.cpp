#include "ui/Renderer.h"
#include "exceptions/GameException.h"

namespace {
const sf::Color ink(90, 82, 73);
const sf::Color lightInk(255, 250, 240);
const sf::Color boardColor(178, 165, 150);
}

Renderer::Renderer(const std::string& fontPath) {
    if (!font.loadFromFile(fontPath)) {
        throw GameException("Cannot load font: " + fontPath
            + ". Place a licensed TrueType font at assets/fonts/2048.ttf; see assets/fonts/README.md.");
    }
}

sf::Color Renderer::backgroundColor() { return sf::Color(248, 245, 239); }

void Renderer::drawCenteredText(sf::RenderWindow& window, const std::string& label,
    unsigned int size, sf::Vector2f center, sf::Color color) const {
    sf::Text text(label, font, size);
    text.setFillColor(color);
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(center);
    window.draw(text);
}

void Renderer::drawScore(sf::RenderWindow& window, const Game& game) const {
    sf::RectangleShape card({280.f, 62.f});
    card.setPosition(50.f, 103.f);
    card.setFillColor(sf::Color(237, 228, 216));
    window.draw(card);
    drawCenteredText(window, "SCORE", 16, {190.f, 118.f}, ink);
    drawCenteredText(window, std::to_string(game.getScore()), 26, {190.f, 145.f}, ink);
    newGameButton.draw(window, font);
}

bool Renderer::restartClicked(const sf::Event& event, sf::Vector2f point,
                              const Game& game) const {
    if (event.type != sf::Event::MouseButtonPressed
        || event.mouseButton.button != sf::Mouse::Left
        || game.getState() == GameState::Paused) { return false; }
    return newGameButton.contains(point) ||
        ((game.getState() == GameState::Won || game.getState() == GameState::Lost)
         && overlayButton.contains(point));
}

void Renderer::drawOverlay(sf::RenderWindow& window, const Game& game) const {
    const bool won = game.getState() == GameState::Won;
    if (!won && game.getState() != GameState::Lost) { return; }
    sf::RectangleShape veil({BOARD_SIZE, BOARD_SIZE});
    veil.setPosition((WINDOW_WIDTH - BOARD_SIZE) / 2.f, BOARD_TOP);
    veil.setFillColor(sf::Color(248, 245, 239, 225));
    window.draw(veil);
    drawCenteredText(window, won ? "You reached 2048!" : "Game Over!", 36,
                     {300.f, 365.f}, ink);
    drawCenteredText(window, won ? "Congratulations, beautifully played!"
                                : "No moves remain. Try again!", 21, {300.f, 415.f}, ink);
    drawCenteredText(window, "Final score: " + std::to_string(game.getScore()), 24,
                     {300.f, 458.f}, ink);
    overlayButton.draw(window, font);
    drawCenteredText(window, "U: Undo  |  R: New Game  |  Esc: Quit", 18,
                     {300.f, 578.f}, ink);
}

void Renderer::render(sf::RenderWindow& window, const Game& game) const {
    drawCenteredText(window, "2048", 56, {WINDOW_WIDTH / 2.f, TITLE_Y}, ink);
    drawScore(window, game);
    drawBoard(window, game.getBoard());
    drawOverlay(window, game);
    const char* state = "Playing";
    switch (game.getState()) {
        case GameState::Playing: break;
        case GameState::Paused: state = "Paused"; break;
        case GameState::Won: state = "Won"; break;
        case GameState::Lost: state = "Game Over"; break;
    }
    drawCenteredText(window, "Move: Arrow Keys / WASD", 17, {300.f, 710.f}, ink);
    drawCenteredText(window, std::string("Undo: U  |  Restart: R  |  Esc: Quit  -  ") + state,
                     15, {300.f, 735.f}, ink);
}

void Renderer::drawBoard(sf::RenderWindow& window, const Board& board) const {
    const float left = (WINDOW_WIDTH - BOARD_SIZE) / 2.f;
    const float cellSize = (BOARD_SIZE - CELL_GAP * (board.getSize() + 1)) / board.getSize();
    sf::RectangleShape area({BOARD_SIZE, BOARD_SIZE});
    area.setPosition(left, BOARD_TOP);
    area.setFillColor(boardColor);
    window.draw(area);
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            drawTile(window, board.getTile(row, col).getValue(),
                {left + CELL_GAP + col * (cellSize + CELL_GAP),
                 BOARD_TOP + CELL_GAP + row * (cellSize + CELL_GAP)}, cellSize);
        }
    }
}

void Renderer::drawTile(sf::RenderWindow& window, int value, sf::Vector2f position, float size) const {
    sf::RectangleShape tile({size, size});
    tile.setPosition(position);
    tile.setFillColor(getTileColor(value));
    window.draw(tile);
    if (value == 0) { return; }
    drawCenteredText(window, std::to_string(value), getTileFontSize(value),
        {position.x + size / 2.f, position.y + size / 2.f}, value <= 4 ? ink : lightInk);
}

unsigned int Renderer::getTileFontSize(int value) const {
    const std::size_t digits = std::to_string(value).size();
    return digits <= 2 ? 44 : digits == 3 ? 36 : digits == 4 ? 29 : 20;
}

sf::Color Renderer::getTileColor(int value) const {
    switch (value) {
        case 0: return sf::Color(204, 193, 179);
        case 2: return sf::Color(238, 228, 218);
        case 4: return sf::Color(237, 224, 200);
        case 8: return sf::Color(229, 177, 130);
        case 16: return sf::Color(221, 151, 110);
        case 32: return sf::Color(213, 126, 97);
        case 64: return sf::Color(199, 99, 79);
        case 128: return sf::Color(202, 176, 100);
        case 256: return sf::Color(192, 163, 80);
        case 512: return sf::Color(180, 150, 61);
        case 1024: return sf::Color(165, 135, 47);
        case 2048: return sf::Color(149, 119, 34);
        default: return sf::Color(96, 91, 84);
    }
}
