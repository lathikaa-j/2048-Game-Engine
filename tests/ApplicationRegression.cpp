#include "ui/Application.h"
#include <iostream>
#include <filesystem>

// Exercises the real screen controller and SFML draw path without touching user saves.
class ApplicationRegression {
    int checks=0, failures=0;
    void check(bool value, const char* label) {
        ++checks; if (!value) { ++failures; std::cerr << "FAIL: " << label << '\n'; }
    }
    sf::Event key(sf::Keyboard::Key code) {
        sf::Event event{}; event.type=sf::Event::KeyPressed; event.key.code=code; return event;
    }
public:
    int run() {
        Application app("assets/fonts/2048.ttf");
        sf::RenderWindow window(sf::VideoMode(600,750),"2048 UI regression",sf::Style::Titlebar|sf::Style::Close);
        window.setVisible(false);
        const auto render=[&](const char* file) {
            window.clear(Renderer::backgroundColor()); app.draw(window);
            sf::Texture capture; capture.create(600,750); capture.update(window);
            check(capture.copyToImage().saveToFile(file),"rendered screen capture");
            window.display();
        };
        check(app.screen==Application::Screen::Main && !app.session.hasGame(),"launch is main menu");
        render("ui-main.png");
        app.menu.click({200,190});
        check(app.screen==Application::Screen::Gameplay && app.session.hasGame(),"Play click starts gameplay");
        const auto initial=app.session.getGame().getBoard();
        app.handle(key(sf::Keyboard::Left),window,false);
        check(app.session.getGame().getBoard()==initial,"unfocused gameplay ignores input");
        app.handle(key(sf::Keyboard::Escape),window,true);
        check(app.screen==Application::Screen::Main && app.session.getGame().getBoard()==initial,"Escape keeps resumable progress");
        app.navigate(Application::Screen::Profiles);
        for (auto c:std::string("Alice")) {
            sf::Event event{}; event.type=sf::Event::TextEntered; event.text.unicode=c; app.handle(event,window,true);
        }
        check(app.name=="Alice" && app.session.getGame().getBoard()==initial,"TextEntered owns profile input");
        sf::Event text{}; text.type=sf::Event::TextEntered; text.text.unicode=8; app.handle(text,window,true);
        check(app.name=="Alic","backspace");
        text.text.unicode='e'; app.handle(text,window,true); text.text.unicode=13; app.handle(text,window,true);
        check(app.session.activePlayer()=="Alice" && app.name.empty(),"Enter creates and selects");
        text.text.unicode=13; app.handle(text,window,true);
        check(!app.message.empty() && app.session.getPlayers().getPlayers().size()==1,"empty profile reports error");
        text.text.unicode='/'; app.handle(text,window,true);
        check(app.name.empty(),"invalid text excluded");
        text.text.unicode='x'; app.handle(text,window,true);
        app.handle(key(sf::Keyboard::Escape),window,true);
        check(app.name.empty() && app.screen==Application::Screen::Profiles,"Escape cancels name entry");
        app.message.clear(); render("ui-profiles.png");
        app.navigate(Application::Screen::Modes); app.menu.click({200,238});
        check(app.session.getSelectedMode().getCurrentMode()==Mode::ENDLESS,"mode button");
        render("ui-modes.png");
        app.navigate(Application::Screen::Main); app.menu.click({200,190});
        check(app.screen==Application::Screen::Gameplay && app.session.sessionPlayer()=="Guest","resume keeps session owner");
        render("ui-gameplay.png");
        app.handle(key(sf::Keyboard::R),window,true);
        check(app.session.getGame().getModes().getCurrentMode()==Mode::STANDARD,"restart keeps session mode");
        app.navigate(Application::Screen::Main); app.menu.click({200,238});
        check(app.session.sessionPlayer()=="Alice" && app.session.getGame().getModes().getCurrentMode()==Mode::ENDLESS,"new game uses selected player and mode");
        app.navigate(Application::Screen::Leaderboard); render("ui-leaderboard.png");
        app.navigate(Application::Screen::Statistics); render("ui-statistics.png");
        app.navigate(Application::Screen::Load); render("ui-load.png");
        const auto board=app.session.getGame().getBoard();
        for (auto code:{sf::Keyboard::W,sf::Keyboard::R,sf::Keyboard::U}) app.handle(key(code),window,true);
        check(app.session.getGame().getBoard()==board,"menu blocks gameplay shortcuts");
        app.handle(key(sf::Keyboard::Escape),window,true);
        check(app.screen==Application::Screen::Main && !app.exitRequested,"secondary Escape returns main");
        app.handle(key(sf::Keyboard::Escape),window,true);
        check(app.exitRequested,"main Escape exits");
        window.close();
        std::cout << checks << " application checks, " << failures << " failures\n";
        return failures?1:0;
    }
};
int main() { return ApplicationRegression{}.run(); }
