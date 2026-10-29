#include "core/Session.h"
#include "core/SaveGame.h"
#include "core/LoadGame.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

std::string Session::activePlayer() const {
    auto name = players.getActivePlayer();
    return name.empty() ? "Guest" : name;
}
int Session::highestTile(const Board& board) {
    int best = 0;
    for (std::size_t r = 0; r < board.getSize(); ++r)
        for (std::size_t c = 0; c < board.getSize(); ++c)
            best = std::max(best, board.getTile(r,c).getValue());
    return best;
}
void Session::selectBoardSize(std::size_t size) {
    if (!Board::isAllowedSize(size)) throw std::invalid_argument("Board size must be 3, 4, or 5");
    selectedBoardSize = size;
}
void Session::finish() {
    if (started) scores.registerScore(owner, game.getScore());
    started = false;
}
void Session::start() {
    finish();
    owner = activePlayer();
    const std::size_t obstacles = obstaclesEnabled
        ? (selectedBoardSize == 3 ? 1 : selectedBoardSize == 4 ? 2 : 3) : 0;
    game = Game(selectedBoardSize, selectedMode, obstacles);
    started = true;
    statistics[owner].incrementGamesPlayed();
    statistics[owner].updateBestTile(highestTile(game.getBoard()));
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
         << ' ' << game.getModes().getTargetTile()
         << " SIZE " << game.getBoard().getSize() << '\n';
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
        std::vector<int> values;
        std::size_t savedSize = 0;
        GameModes mode;
        std::string token;
        while (input >> token) {
            if (token == "MODE") {
                int modeValue, target;
                if (!(input >> modeValue >> target)) throw std::invalid_argument("Invalid mode metadata");
                mode.setMode(static_cast<Mode>(modeValue), target);
            } else if (token == "SIZE") {
                int size;
                if (!(input >> size) || size < 0) throw std::invalid_argument("Invalid board size metadata");
                savedSize = static_cast<std::size_t>(size);
            } else {
                std::size_t used = 0;
                int value;
                try { value = std::stoi(token, &used); }
                catch (...) { throw std::invalid_argument("Invalid grid"); }
                if (used != token.size() || value < -1
                    || (value != 0 && value != -1 && (value < 2 || (value & (value - 1)))))
                    throw std::invalid_argument("Invalid grid");
                values.push_back(value);
            }
        }
        if (savedSize == 0) {
            for (std::size_t candidate = Board::MIN_BOARD_SIZE; candidate <= Board::MAX_BOARD_SIZE; ++candidate)
                if (values.size() == candidate * candidate) savedSize = candidate;
        }
        if (!Board::isAllowedSize(savedSize) || values.size() != savedSize * savedSize)
            throw std::invalid_argument("Invalid grid dimensions");
        Board board(savedSize);
        for (std::size_t i = 0; i < values.size(); ++i) {
            board.setTile(i / savedSize, i % savedSize, values[i]);
        }
        if (!highestTile(board)) throw std::invalid_argument("Empty grid");
        Game restored(board, score, mode); // Validate completely before replacing progress.
        finish();
        game = restored;
        owner = activePlayer();
        selectedMode = mode;
        selectedBoardSize = savedSize;
        obstaclesEnabled = board.getObstacleCount() != 0;
        started = true;
        statistics[owner].updateBestTile(highestTile(board));
        message = "Loaded " + owner + "_save.txt";
        return true;
    } catch (const std::exception&) {
        message = "Invalid save. Current progress was preserved.";
        return false;
    }
}
