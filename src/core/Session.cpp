#include "core/Session.h"
#include "core/SaveGame.h"
#include "core/LoadGame.h"
#include <sstream>
#include <algorithm>

std::string Session::activePlayer() const {
    auto name = players.getActivePlayer();
    return name.empty() ? "Guest" : name;
}
int Session::highestTile(const Board& board) {
    int best = 0;
    for (std::size_t r = 0; r < 4; ++r)
        for (std::size_t c = 0; c < 4; ++c)
            best = std::max(best, board.getTile(r,c).getValue());
    return best;
}
void Session::finish() {
    if (started) scores.registerScore(owner, game.getScore());
    started = false;
}
void Session::start() {
    finish();
    owner = activePlayer();
    Board initial;
    initial.addRandomTile(); initial.addRandomTile();
    game = Game(initial, 0, selectedMode);
    started = true;
    statistics[owner].incrementGamesPlayed();
    statistics[owner].updateBestTile(highestTile(initial));
}
void Session::restart() {
    // Restart the current player's current mode, even if menu selections changed.
    finish();
    game.newGame();
    started = true;
    statistics[owner].incrementGamesPlayed();
    statistics[owner].updateBestTile(highestTile(game.getBoard()));
}
void Session::observeMove(const Board& before) {
    if (game.getBoard() != before) {
        statistics[owner].incrementMoves();
        statistics[owner].updateBestTile(highestTile(game.getBoard()));
        if (game.getState() == GameState::Won || game.getState() == GameState::Lost)
            scores.registerScore(owner, game.getScore());
    }
}
bool Session::save(std::string& message) const {
    if (!started) { message = "Start a game before saving."; return false; }
    std::ostringstream data;
    data << game.getBoard();
    // Optional trailer in the existing opaque board-data field. Legacy grids still load.
    data << "MODE " << static_cast<int>(game.getModes().getCurrentMode())
         << ' ' << game.getModes().getTargetTile() << '\n';
    SaveGame writer;
    const bool ok = writer.saveState(owner, game.getScore(), data.str());
    message = ok ? "Saved " + owner + "_save.txt" : "Save failed. Check file permissions.";
    return ok;
}
bool Session::load(std::string& message) {
    int score = 0;
    std::string data;
    LoadGame reader;
    if (!reader.loadState(activePlayer(), score, data)) {
        message = "Missing or invalid " + activePlayer() + "_save.txt"; return false;
    }
    try {
        std::istringstream input(data);
        Board board;
        for (std::size_t r = 0; r < 4; ++r) {
            for (std::size_t c = 0; c < 4; ++c) {
                int value;
                if (!(input >> value) || value < 0
                    || (value != 0 && (value < 2 || (value & (value - 1)))))
                    throw std::invalid_argument("Invalid grid");
                board.setTile(r,c,value);
            }
        }
        if (!highestTile(board)) throw std::invalid_argument("Empty grid");
        GameModes mode;
        input >> std::ws;
        if (!input.eof()) {
            std::string tag; int modeValue, target;
            if (!(input >> tag >> modeValue >> target) || tag != "MODE")
                throw std::invalid_argument("Invalid metadata");
            mode.setMode(static_cast<Mode>(modeValue), target);
            input >> std::ws;
            if (!input.eof()) throw std::invalid_argument("Unexpected data");
        }
        Game restored(board, score, mode); // Validate completely before replacing progress.
        finish();
        game = restored;
        owner = activePlayer();
        selectedMode = mode;
        started = true;
        statistics[owner].updateBestTile(highestTile(board));
        message = "Loaded " + owner + "_save.txt";
        return true;
    } catch (const std::exception&) {
        message = "Invalid save. Current progress was preserved.";
        return false;
    }
}
