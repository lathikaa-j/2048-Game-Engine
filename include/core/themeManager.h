#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <string>
#include <map>
#include <vector>

// UI-Agnostic color structure
struct RGBColor {
    unsigned char r, g, b;
};

// Holds all styling data for a single visual theme
struct Theme {
    std::string name;
    RGBColor backgroundColor;
    RGBColor gridColor;
    RGBColor emptyTileColor;
    RGBColor defaultTextColor;
    std::map<int, RGBColor> tileColors;     // Maps tile value (e.g., 2048) to its background color
    std::map<int, RGBColor> tileTextColors; // Maps tile value to its text color
};

class ThemeManager {
private:
    std::map<std::string, Theme> themes;
    std::string currentThemeName;

    // Private helper to populate the themes map on initialization
    void loadDefaultThemes();

public:
    ThemeManager();

    // Core functionality
    bool setTheme(const std::string& themeName);
    Theme getCurrentTheme() const;
    std::vector<std::string> getAvailableThemes() const;
};

#endif // THEME_MANAGER_H