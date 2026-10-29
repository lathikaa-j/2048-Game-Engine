#pragma once
#include "core/Session.h"
#include "ui/Renderer.h"
#include "ui/InputHandler.h"
#include <functional>
#include <vector>

class MenuScreen {
    struct Action { Button button; std::function<void()> invoke; };
    std::vector<Action> actions;
public:
    void clear() { actions.clear(); }
    void add(const std::string& label, std::function<void()> action);
    void draw(sf::RenderWindow& window, const sf::Font& font) const;
    void click(sf::Vector2f point);
};
class Application {
    friend class ApplicationRegression;
    enum class Screen { Main, Profiles, Modes, SelectTarget, BoardSizes, Themes, Gameplay, Load, Leaderboard, Statistics };
    sf::Font font;
    Renderer renderer;
    InputHandler input;
    Session session;
    ThemeManager themeManager;
    MenuScreen menu;
    Screen screen = Screen::Main;
    std::string name, message, targetInput;
    std::size_t profilePage = 0;
    bool exitRequested = false;
    Button menuButton{{20, 12, 100, 32}, "Menu"};
    Button saveButton{{470, 12, 110, 32}, "Save"};
    Button confirmTargetButton{{100, 350, 180, 46}, "Confirm"};
    Button backTargetButton{{320, 350, 180, 46}, "Back"};
    void navigate(Screen next);
    void rebuildMenu();
    void text(sf::RenderWindow& window, const std::string& value, float y, unsigned size = 20) const;
    void draw(sf::RenderWindow& window);
    void createProfile();
    void confirmTarget();
    std::string themeName() const { return themeManager.getCurrentTheme().name; }
    void handle(const sf::Event& event, sf::RenderWindow& window, bool focused);
public:
    explicit Application(const std::string& fontPath);
    int run();
};
