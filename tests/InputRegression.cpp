#include "ui/InputHandler.h"
#include "ui/Renderer.h"
#include <iostream>
#include <vector>

namespace {
int failures = 0;
int checks = 0;
void check(bool condition, const char* name) {
    ++checks;
    if (!condition) { ++failures; std::cerr << "FAIL: " << name << '\n'; }
}
sf::Event key(sf::Keyboard::Key code) {
    sf::Event event{};
    event.type = sf::Event::KeyPressed;
    event.key.code = code;
    return event;
}
bool fresh(const Game& game) {
    int occupied = 0;
    for (std::size_t r = 0; r < 4; ++r) {
        for (std::size_t c = 0; c < 4; ++c) {
            const int value = game.getBoard().getTile(r, c).getValue();
            if (value) { ++occupied; if (value != 2 && value != 4) { return false; } }
        }
    }
    return occupied == 2 && game.getScore() == 0 && game.getState() == GameState::Playing;
}
}

int main() {
    InputHandler input;
    Board seed;
    seed.setTile(1, 1, 2);
    seed.setTile(1, 2, 2);
    const std::vector<std::pair<sf::Keyboard::Key, Direction>> mappings = {
        {sf::Keyboard::Up, Direction::Up}, {sf::Keyboard::W, Direction::Up},
        {sf::Keyboard::Down, Direction::Down}, {sf::Keyboard::S, Direction::Down},
        {sf::Keyboard::Left, Direction::Left}, {sf::Keyboard::A, Direction::Left},
        {sf::Keyboard::Right, Direction::Right}, {sf::Keyboard::D, Direction::Right}
    };
    for (const auto& mapping : mappings) {
        Game actual(seed), expected = actual; // Board copies preserve RNG state.
        input.handleEvent(key(mapping.first), actual);
        expected.move(mapping.second);
        check(actual.getBoard() == expected.getBoard() && actual.getScore() == expected.getScore(),
              "Arrow/WASD delegates exact direction, score and single spawn");
        input.handleEvent(key(sf::Keyboard::U), actual);
        check(actual.getBoard() == seed && actual.getScore() == 0 && !actual.undo(),
              "U restores board/score and consumes one history entry");
    }
    Game game(seed);
    check(input.handleEvent(key(sf::Keyboard::Space), game) == InputHandler::Result::Ignored
          && game.getBoard() == seed && !game.undo(), "Unsupported key ignored");
    for (auto code : {sf::Keyboard::Left, sf::Keyboard::U, sf::Keyboard::R, sf::Keyboard::Escape}) {
        check(input.handleEvent(key(code), game, false) == InputHandler::Result::Ignored
              && game.getBoard() == seed, "Menu/modal/unfocused ownership blocks shortcuts");
    }
    auto released = key(sf::Keyboard::Left);
    released.type = sf::Event::KeyReleased;
    input.handleEvent(released, game);
    check(game.getBoard() == seed, "Key release ignored");
    game.pause();
    input.handleEvent(key(sf::Keyboard::R), game);
    check(game.getState() == GameState::Paused && game.getBoard() == seed, "Paused screen owns input");
    game.resume();
    check(input.handleEvent(key(sf::Keyboard::Escape), game) == InputHandler::Result::CloseRequested,
          "Escape requests application close");

    Board blocked;
    for (std::size_t c = 0; c < 4; ++c) { blocked.setTile(0, c, 2 << c); }
    Game noChange(blocked, 12);
    input.handleEvent(key(sf::Keyboard::Left), noChange);
    check(noChange.getBoard() == blocked && noChange.getScore() == 12 && !noChange.undo(),
          "Blocked input leaves score, board and history unchanged");

    Board winning;
    winning.setTile(0, 0, 1024);
    winning.setTile(0, 1, 1024);
    Game win(winning, 100);
    input.handleEvent(key(sf::Keyboard::Left), win);
    check(win.getState() == GameState::Won && win.getScore() == 2148, "Controlled merge reaches Won");
    const Board won = win.getBoard();
    input.handleEvent(key(sf::Keyboard::Right), win);
    check(win.getBoard() == won && win.getScore() == 2148, "Won blocks ordinary moves");
    input.handleEvent(key(sf::Keyboard::U), win);
    check(win.getBoard() == winning && win.getScore() == 100
          && win.getState() == GameState::Playing, "Undo dismisses win via engine state");
    input.handleEvent(key(sf::Keyboard::Left), win);
    input.handleEvent(key(sf::Keyboard::R), win);
    check(fresh(win) && !win.undo(), "Restart after winning resets all session state");

    Board losing;
    const int values[4][4] = {{2,4,8,16}, {32,64,128,256}, {512,1024,2,4}, {8,16,32,64}};
    for (std::size_t r = 0; r < 4; ++r) {
        for (std::size_t c = 0; c < 4; ++c) { losing.setTile(r,c,values[r][c]); }
    }
    Game loss(losing, 200);
    check(loss.getState() == GameState::Lost, "Controlled full board is Lost");
    for (const auto& mapping : mappings) { input.handleEvent(key(mapping.first), loss); }
    check(loss.getBoard() == losing && loss.getScore() == 200 && !loss.undo(), "Lost blocks every move");
    input.handleEvent(key(sf::Keyboard::R), loss);
    check(fresh(loss) && !loss.undo(), "Restart after loss");
    // A slide leaves exactly one empty cell. Either possible spawn makes a dead board.
    losing.setTile(3, 2, 0);
    Game lastMove(losing, 200);
    input.handleEvent(key(sf::Keyboard::Right), lastMove);
    check(lastMove.getState() == GameState::Lost && lastMove.getScore() == 200,
          "Last valid slide and spawn transition to Lost deterministically");
    input.handleEvent(key(sf::Keyboard::U), lastMove);
    check(lastMove.getBoard() == losing && lastMove.getState() == GameState::Playing,
          "Undo after loss restores playing state");
    for (int i = 0; i < 30; ++i) {
        input.handleEvent(key(sf::Keyboard::Right), loss);
        input.handleEvent(key(sf::Keyboard::U), loss);
        input.handleEvent(key(sf::Keyboard::R), loss);
        check(fresh(loss) && !loss.undo(), "Repeated restart after undo clears history");
    }
    Button button({10, 20, 100, 40}, "Test");
    check(button.contains({50, 30}) && !button.contains({0, 0}), "Button hit geometry");
    Renderer renderer("assets/fonts/2048.ttf");
    sf::Event click{};
    click.type = sf::Event::MouseButtonPressed;
    click.mouseButton.button = sf::Mouse::Left;
    Game playing(seed), terminal(won);
    check(renderer.restartClicked(click, {450, 135}, playing), "Header restart hit target");
    check(!renderer.restartClicked(click, {300, 520}, playing)
          && renderer.restartClicked(click, {300, 520}, terminal), "Overlay button only active at end");
    click.mouseButton.button = sf::Mouse::Right;
    check(!renderer.restartClicked(click, {450, 135}, playing), "Right click ignored");
    click.mouseButton.button = sf::Mouse::Left;
    playing.pause();
    check(!renderer.restartClicked(click, {450, 135}, playing), "Paused screen blocks mouse restart");
    std::cout << checks << " checks, " << failures << " failures\n";
    return failures ? 1 : 0;
}
