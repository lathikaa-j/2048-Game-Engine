#include "core/themeManager.h"

ThemeManager::ThemeManager() {
    loadDefaultThemes();
    // Set a safe default theme fallback
    currentThemeName = "Classic";
}

void ThemeManager::loadDefaultThemes() {
    // 1. Classic Theme (Standard 2048 Colors)
    Theme classicTheme;
    classicTheme.name = "Classic";
    classicTheme.backgroundColor = {250, 248, 239};
    classicTheme.gridColor = {187, 173, 160};
    classicTheme.emptyTileColor = {205, 193, 180};
    classicTheme.defaultTextColor = {119, 110, 101};
    
    classicTheme.tileColors[2]    = {238, 228, 218};
    classicTheme.tileColors[4]    = {237, 224, 200};
    classicTheme.tileColors[8]    = {242, 177, 121};
    classicTheme.tileColors[2048] = {237, 194, 46};
    
    themes["Classic"] = classicTheme;

    // 2. Dark Mode Theme
    Theme darkTheme;
    darkTheme.name = "Dark";
    darkTheme.backgroundColor = {30, 30, 30};
    darkTheme.gridColor = {50, 50, 50};
    darkTheme.emptyTileColor = {70, 70, 70};
    darkTheme.defaultTextColor = {220, 220, 220};
    
    darkTheme.tileColors[2]    = {80, 80, 80};
    darkTheme.tileColors[4]    = {100, 100, 100};
    darkTheme.tileColors[8]    = {120, 80, 80};
    darkTheme.tileColors[2048] = {200, 150, 30};
    
    themes["Dark"] = darkTheme;

    // 3. Neon Theme
    Theme neonTheme;
    neonTheme.name = "Neon";
    neonTheme.backgroundColor = {10, 10, 25};
    neonTheme.gridColor = {20, 20, 50};
    neonTheme.emptyTileColor = {30, 30, 70};
    neonTheme.defaultTextColor = {255, 255, 255};
    
    neonTheme.tileColors[2]    = {0, 255, 255};   // Cyan
    neonTheme.tileColors[4]    = {255, 0, 255};   // Magenta
    neonTheme.tileColors[8]    = {0, 255, 128};   // Spring Green
    neonTheme.tileColors[2048] = {255, 255, 0};   // Yellow
    
    themes["Neon"] = neonTheme;
}

bool ThemeManager::setTheme(const std::string& themeName) {
    // Check if the requested theme exists in our map
    if (themes.find(themeName) != themes.end()) {
        currentThemeName = themeName;
        return true;
    }
    return false; // Theme not found
}

Theme ThemeManager::getCurrentTheme() const {
    // std::map::at() is safe here because currentThemeName is strictly controlled by setTheme()
    return themes.at(currentThemeName);
}

std::vector<std::string> ThemeManager::getAvailableThemes() const {
    std::vector<std::string> themeNames;
    for (const auto& pair : themes) {
        themeNames.push_back(pair.first);
    }
    return themeNames;
}