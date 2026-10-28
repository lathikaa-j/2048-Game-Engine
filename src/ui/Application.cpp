#include "ui/Application.h"
#include "exceptions/GameException.h"
#include <algorithm>

namespace {
std::string modeName(const GameModes& mode) {
    switch (mode.getCurrentMode()) {
        case Mode::ENDLESS: return "Endless";
        case Mode::TARGET: return "Target " + std::to_string(mode.getTargetTile());
        default: return "Standard 2048";
    }
}
}
void MenuScreen::add(const std::string& label, std::function<void()> action) {
    actions.push_back({Button({50.f, 170.f + 48.f * actions.size(), 500.f, 40.f}, label), std::move(action)});
}
void MenuScreen::draw(sf::RenderWindow& window, const sf::Font& font) const {
    for (const auto& action : actions) action.button.draw(window, font);
}
void MenuScreen::click(sf::Vector2f point) {
    for (const auto& action : actions) if (action.button.contains(point)) {
        auto callback = action.invoke; // Navigation can rebuild this list.
        callback(); return;
    }
}
Application::Application(const std::string& path) : renderer(path) {
    if (!font.loadFromFile(path)) throw GameException("Cannot load menu font: " + path);
    rebuildMenu();
}
void Application::navigate(Screen next) {
    screen = next; name.clear(); message.clear(); rebuildMenu();
}
void Application::createProfile() {
    if (!session.getPlayers().createProfile(name))
        message = "Use 1-20 letters, digits, _ or -.\nNames must be unique; Guest is reserved.";
    else {
        session.getPlayers().selectPlayer(name);
        message = "Selected " + name + ". Applies to new games.";
        name.clear(); rebuildMenu();
    }
}
void Application::rebuildMenu() {
    menu.clear();
    auto back = [this] { navigate(Screen::Main); };
    switch (screen) {
        case Screen::Main:
            if (session.hasGame()) menu.add("Resume Game", [this] { navigate(Screen::Gameplay); });
            menu.add("Play New Game", [this] { session.start(); navigate(Screen::Gameplay); });
            menu.add("Player Profiles", [this] { navigate(Screen::Profiles); });
            menu.add("Game Modes", [this] { navigate(Screen::Modes); });
            menu.add("Load Game", [this] { navigate(Screen::Load); });
            menu.add("Leaderboard", [this] { navigate(Screen::Leaderboard); });
            menu.add("Statistics", [this] { navigate(Screen::Statistics); });
            menu.add("Exit", [this] { exitRequested = true; }); break;
        case Screen::Profiles: {
            menu.add("Create & Select (Enter)", [this] { createProfile(); });
            const auto names = session.getPlayers().getPlayers();
            if (profilePage >= names.size()) profilePage = names.empty() ? 0 : ((names.size()-1)/5)*5;
            for (std::size_t i = profilePage; i < std::min(names.size(), profilePage+5); ++i) {
                const auto player = names[i];
                menu.add(player + (player == session.activePlayer() ? " [selected]" : ""), [this, player] {
                    session.getPlayers().selectPlayer(player);
                    message = "Selected " + player + ". Applies to new games."; rebuildMenu();
                });
            }
            if (profilePage) menu.add("Previous profiles", [this] { profilePage -= 5; rebuildMenu(); });
            if (profilePage+5 < names.size()) menu.add("More profiles", [this] { profilePage += 5; rebuildMenu(); });
            menu.add("Back", back); break;
        }
        case Screen::Modes:
            menu.add("Standard - reach 2048", [this] { session.selectMode(Mode::STANDARD); message = "Standard selected for new games."; });
            menu.add("Endless - play until no moves remain", [this] { session.selectMode(Mode::ENDLESS); message = "Endless selected for new games."; });
            menu.add("Target - reach 512", [this] { session.selectMode(Mode::TARGET,512); message = "Target 512 selected for new games."; });
            menu.add("Back", back); break;
        case Screen::Load:
            menu.add("Load selected player's save", [this] {
                std::string result;
                if (session.load(result)) { navigate(Screen::Gameplay); message = result; }
                else message = result;
            });
            menu.add("Back", back); break;
        case Screen::Leaderboard: case Screen::Statistics: menu.add("Back", back); break;
        case Screen::Gameplay: break;
    }
}
void Application::text(sf::RenderWindow& window, const std::string& value, float y, unsigned size) const {
    sf::Text label(value,font,size); label.setFillColor(sf::Color(90,82,73));
    const auto bounds = label.getLocalBounds();
    label.setPosition(300.f - (bounds.left+bounds.width/2),y); window.draw(label);
}
void Application::draw(sf::RenderWindow& window) {
    if (screen == Screen::Gameplay) {
        renderer.render(window,session.getGame()); menuButton.draw(window,font); saveButton.draw(window,font);
        text(window,message.empty() ? session.sessionPlayer()+" | "+modeName(session.getGame().getModes()) : message,172,15);
        return;
    }
    const char* title = "2048";
    switch (screen) {
        case Screen::Profiles: title="Player Profiles"; break;
        case Screen::Modes: title="Game Modes"; break;
        case Screen::Load: title="Load Game"; break;
        case Screen::Leaderboard: title="Leaderboard"; break;
        case Screen::Statistics: title="Statistics"; break;
        default: break;
    }
    text(window,title,45,40);
    text(window,"Player: "+session.activePlayer()+" | "+modeName(session.getSelectedMode()),105,18);
    menu.draw(window,font);
    if (screen == Screen::Profiles) {
        sf::RectangleShape field({500,32}); field.setPosition(50,133);
        field.setFillColor(sf::Color(237,228,216)); window.draw(field);
        text(window,"Name: "+name+"_",135,19);
        text(window,"Profiles last for this application session.",610,17);
    } else if (screen == Screen::Load) {
        text(window,"File: "+session.activePlayer()+"_save.txt",260);
        text(window,"Loading replaces progress after validation.",310,17);
        text(window,"Legacy grids load in Standard mode.",345,17);
    } else if (screen == Screen::Leaderboard) {
        text(window,"Best per player - this application session",240,18);
        const auto& entries = session.getScores().getEntries();
        if (entries.empty()) text(window,"No retired games yet.",290,19);
        for (std::size_t i=0; i<entries.size(); ++i)
            text(window,std::to_string(i+1)+". "+entries[i].username+"   "+std::to_string(entries[i].score),285.f+28.f*i,19);
        text(window,"Records update on completion, restart or exit.",610,17);
    } else if (screen == Screen::Statistics) {
        const auto& stats = session.getStatistics(session.activePlayer());
        text(window,"This application session / selected player",250,18);
        text(window,"Games started: "+std::to_string(stats.getGamesPlayed()),310);
        text(window,"Successful moves: "+std::to_string(stats.getMoves()),360);
        text(window,"Best tile observed: "+std::to_string(stats.getBestTile()),410);
        text(window,"Undo does not subtract moves or best tile.",470,17);
    }
    text(window,message,650,17); text(window,"Esc: Back / Exit",715,17);
}
void Application::handle(const sf::Event& event, sf::RenderWindow& window, bool focused) {
    if (!focused) return;
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (screen == Screen::Main) exitRequested=true;
        else if (screen == Screen::Profiles && !name.empty()) name.clear();
        else navigate(Screen::Main);
        return;
    }
    if (screen == Screen::Profiles && event.type == sf::Event::TextEntered) {
        const auto code=event.text.unicode;
        if (code==8) { if (!name.empty()) name.pop_back(); }
        else if (code==13) createProfile();
        else if (code>=32) {
            if (code<128 && name.size()<20 && std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-").find(static_cast<char>(code)) != std::string::npos)
                name+=static_cast<char>(code);
            else message="Use up to 20 letters, digits, _ or -.";
        }
        return;
    }
    const bool click=event.type==sf::Event::MouseButtonPressed && event.mouseButton.button==sf::Mouse::Left;
    sf::Vector2f point;
    if (click) point=window.mapPixelToCoords({event.mouseButton.x,event.mouseButton.y});
    if (screen!=Screen::Gameplay) { if (click) menu.click(point); return; }
    if (click && menuButton.contains(point)) { navigate(Screen::Main); return; }
    if (click && saveButton.contains(point)) { session.save(message); return; }
    if ((event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::R)
        || (click && renderer.restartClicked(event,point,session.getGame()))) {
        session.restart(); message.clear(); return;
    }
    const Board before=session.getGame().getBoard();
    input.handleEvent(event,session.getGame(),true);
    if (!(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::U)) session.observeMove(before);
    if (session.getGame().getBoard()!=before) message.clear();
}
int Application::run() {
    sf::RenderWindow window(sf::VideoMode(Renderer::WINDOW_WIDTH,Renderer::WINDOW_HEIGHT),
        "2048 Game Engine",sf::Style::Titlebar|sf::Style::Close);
    window.setFramerateLimit(60); window.setKeyRepeatEnabled(false);
    bool focused=window.hasFocus();
    while (window.isOpen() && !exitRequested) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type==sf::Event::Closed) { exitRequested=true; break; }
            if (event.type==sf::Event::LostFocus) focused=false;
            if (event.type==sf::Event::GainedFocus) focused=true;
            try { handle(event,window,focused); }
            catch (const std::exception& error) { message=error.what(); }
            if (exitRequested) break;
        }
        if (exitRequested) break;
        window.clear(Renderer::backgroundColor()); draw(window); window.display();
    }
    session.close(); window.close(); return 0;
}
