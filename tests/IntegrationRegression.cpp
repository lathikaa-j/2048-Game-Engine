#include "core/Session.h"
#include "core/LoadGame.h"
#include "core/FileManager.h"
#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
namespace fs = std::filesystem;
int checks=0, failures=0;
void check(bool condition, const char* label) {
    ++checks; if (!condition) { ++failures; std::cerr << "FAIL: " << label << '\n'; }
}
struct TemporaryDirectory {
    fs::path original=fs::current_path();
    fs::path path=fs::temp_directory_path()/ ("2048-integration-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    TemporaryDirectory() { fs::create_directory(path); fs::current_path(path); }
    ~TemporaryDirectory() { fs::current_path(original); fs::remove_all(path); }
};
int main(int argc, char* argv[]) {
    if (argc == 2) {
        Session fixture;
        fixture.getPlayers().createProfile("IntegrationFixture");
        fixture.getPlayers().selectPlayer("IntegrationFixture");
        std::string message;
        if (std::string(argv[1]) == "write") {
            fixture.selectMode(Mode::TARGET,512); fixture.start();
            Board board; board.setTile(0,0,512); board.setTile(3,3,4);
            fixture.getGame()=Game(board,1234,fixture.getSelectedMode());
            return fixture.save(message) ? 0 : 1;
        }
        const bool loaded=fixture.load(message);
        const bool valid=loaded && fixture.getGame().getBoard().getTile(0,0).getValue()==512
            && fixture.getGame().getBoard().getTile(3,3).getValue()==4
            && fixture.getGame().getScore()==1234 && fixture.getGame().getState()==GameState::Won
            && fixture.getGame().getModes().getCurrentMode()==Mode::TARGET
            && fixture.getGame().getModes().getTargetTile()==512
            && fixture.sessionPlayer()=="IntegrationFixture";
        return valid ? 0 : 1;
    }
    TemporaryDirectory temporary;
    Session session;
    auto& players=session.getPlayers();
    check(!players.createProfile("") && !players.createProfile("../bad") && !players.createProfile("Guest") && !players.createProfile("a b"),"invalid profiles");
    check(players.createProfile("Alice") && !players.createProfile("Alice") && players.createProfile("Bob"),"create and duplicate profiles");
    check(!players.createProfile("alice") && !players.createProfile("guest"),"case aliases cannot overwrite another save slot");
    check(players.getPlayers()==std::vector<std::string>({"Alice","Bob"}),"sorted profiles");
    check(!players.selectPlayer("missing") && players.selectPlayer("Alice"),"selection rules");
    session.selectMode(Mode::ENDLESS); session.start();
    check(session.sessionPlayer()=="Alice" && session.getStatistics("Alice").getGamesPlayed()==1,"session owner and start statistics");
    Board initial; initial.setTile(0,0,1024); initial.setTile(0,1,1024);
    GameModes endless; endless.setMode(Mode::ENDLESS);
    session.getGame()=Game(initial,12,endless);
    auto before=session.getGame().getBoard();
    session.getGame().move(Direction::Left); session.observeMove(before);
    check(session.getGame().getScore()==2060 && session.getGame().getState()==GameState::Playing,"endless continues past 2048");
    check(session.getStatistics("Alice").getMoves()==1 && session.getStatistics("Alice").getBestTile()==2048,"move statistics");
    auto saved=session.getGame(); std::string message;
    check(session.save(message) && fs::exists("Alice_save.txt"),"save existing format");
    { // A fresh application model proves persistence independent of in-memory state.
        Session fresh; fresh.getPlayers().createProfile("Alice"); fresh.getPlayers().selectPlayer("Alice");
        check(fresh.load(message) && fresh.getGame().getBoard()==saved.getBoard() && fresh.getGame().getScore()==2060
            && fresh.getGame().getState()==saved.getState() && fresh.getGame().getModes().getCurrentMode()==Mode::ENDLESS,"fresh-session load restores grid score mode state");
        check(!fresh.getGame().undo(),"load clears undo history");
    }
    session.getGame().undo();
    check(session.getStatistics("Alice").getMoves()==1,"undo does not decrement activity metrics");
    check(session.load(message) && session.getGame().getBoard()==saved.getBoard(),"reload snapshot");
    players.selectPlayer("Bob"); session.selectMode(Mode::TARGET,512); session.restart();
    check(session.sessionPlayer()=="Alice" && session.getGame().getModes().getCurrentMode()==Mode::ENDLESS,"restart preserves actual player and mode");
    session.start();
    check(session.sessionPlayer()=="Bob" && session.getGame().getModes().getCurrentMode()==Mode::TARGET,"new game uses selections");
    check(!session.load(message) && session.sessionPlayer()=="Bob","missing save preserves session");
    Board seed; seed.setTile(0,0,256); seed.setTile(0,1,256);
    GameModes target; target.setMode(Mode::TARGET,512);
    session.getGame()=Game(seed,10,target); before=seed;
    session.getGame().move(Direction::Left); session.observeMove(before);
    check(session.getGame().getState()==GameState::Won,"target wins at 512");
    check(session.getScores().getEntries().size()==2,"completion and retirement records");
    saved=session.getGame(); check(session.save(message),"save terminal target game");
    check(session.load(message) && session.getGame().getState()==GameState::Won && session.getGame().getModes().getTargetTile()==512,"target terminal load");
    const std::string grid="2 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n";
    const std::vector<std::string> corrupt={"", "nonsense\n"+grid,"-1\n"+grid,"3junk\n"+grid,"999999999999999\n"+grid,
        "0\n2 0", "0\n3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0", "0\n"+grid+"MODE 9 512", "0\n"+grid+"MODE 2 7", "0\n"+grid+"extra"};
    for (const auto& content:corrupt) {
        FileManager::writeToFile("Bob_save.txt",content);
        check(!session.load(message) && session.getGame().getBoard()==saved.getBoard() && session.getGame().getScore()==saved.getScore()
            && session.getGame().getState()==saved.getState(),"corrupt save is transactional");
    }
    FileManager::writeToFile("Bob_save.txt","40\n"+grid);
    check(session.load(message) && session.getGame().getScore()==40 && session.getGame().getModes().getCurrentMode()==Mode::STANDARD,"legacy board save");
    session.close(); session.close();
    const auto& entries=session.getScores().getEntries();
    check(entries.size()==2 && entries[0].username=="Alice" && entries[0].score==2060 && entries[1].score==522,"best per player ordered without duplicates");
    HighScoreSystem leaderboard;
    for (int i=0;i<15;++i) leaderboard.registerScore("P"+std::to_string(i),i*10);
    check(leaderboard.getEntries().size()==10 && leaderboard.getEntries().front().score==140 && leaderboard.getEntries().back().score==50,"top ten descending");
    bool rejected=false; try { target.setMode(Mode::TARGET,7); } catch (const std::invalid_argument&) { rejected=true; }
    check(rejected && target.getTargetTile()==512,"invalid mode preserves settings");
    Game overflow(seed,std::numeric_limits<int>::max());
    rejected=false; try { overflow.move(Direction::Left); } catch (const std::exception&) { rejected=true; }
    check(rejected && overflow.getBoard()==seed && !overflow.undo(),"score overflow preserves progress");
    Board dead; int values[16]={2,4,8,16,32,64,128,256,512,1024,2,4,8,16,32,64};
    for (int i=0;i<16;++i) dead.setTile(i/4,i%4,values[i]);
    check(Game(dead,0,endless).getState()==GameState::Lost,"endless still loses on dead board");
    Game targetUndo(seed,0,target); targetUndo.move(Direction::Left);
    check(targetUndo.undo() && targetUndo.getState()==GameState::Playing && targetUndo.getBoard()==seed,"target undo restores state");
    GameModes tinyTarget; tinyTarget.setMode(Mode::TARGET,2);
    Game tiny(seed,0,tinyTarget); tiny.newGame();
    check(tiny.getState()==GameState::Won,"restart evaluates small target immediately");
    std::cout << checks << " integration checks, " << failures << " failures\n";
    return failures ? 1:0;
}
