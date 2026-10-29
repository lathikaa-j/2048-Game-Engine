#include "core/themeManager.h"

ThemeManager::ThemeManager() {
    loadDefaultThemes();
    currentThemeName = "Classic";
}

void ThemeManager::loadDefaultThemes() {
    Theme classicTheme;
    classicTheme.name = "Classic";
    classicTheme.backgroundColor = {250, 248, 239};
    classicTheme.gridColor = {187, 173, 160};
    classicTheme.emptyTileColor = {205, 193, 180};
    classicTheme.defaultTextColor = {119, 110, 101};
    classicTheme.tileColors[2] = {238, 228, 218};
    classicTheme.tileColors[4] = {237, 224, 200};
    classicTheme.tileColors[8] = {242, 177, 121};
    classicTheme.tileColors[2048] = {237, 194, 46};
    themes["Classic"] = classicTheme;

    Theme darkTheme;
    darkTheme.name = "Dark";
    darkTheme.backgroundColor = {30, 30, 30};
    darkTheme.gridColor = {50, 50, 50};
    darkTheme.emptyTileColor = {70, 70, 70};
    darkTheme.defaultTextColor = {220, 220, 220};
    darkTheme.tileColors[2] = {60, 90, 110};
    darkTheme.tileColors[4] = {50, 120, 130};
    darkTheme.tileColors[8] = {120, 80, 80};
    darkTheme.tileColors[2048] = {200, 150, 30};
    themes["Dark"] = darkTheme;

    Theme roseGold;
    roseGold.name = "RoseGold";
    roseGold.backgroundColor = {253, 246, 245};
    roseGold.gridColor = {224, 196, 190};
    roseGold.emptyTileColor = {243, 224, 220};
    roseGold.defaultTextColor = {138, 102, 100};
    roseGold.tileColors[2] = {249, 212, 211};
    roseGold.tileColors[4] = {244, 192, 192};
    roseGold.tileColors[8] = {239, 172, 172};
    roseGold.tileColors[2048] = {212, 161, 170};
    themes["RoseGold"] = roseGold;

    Theme matcha;
    matcha.name = "Matcha";
    matcha.backgroundColor = {244, 247, 240};
    matcha.gridColor = {195, 204, 185};
    matcha.emptyTileColor = {222, 230, 213};
    matcha.defaultTextColor = {85, 107, 75};
    matcha.tileColors[2] = {207, 222, 196};
    matcha.tileColors[4] = {184, 209, 168};
    matcha.tileColors[8] = {162, 196, 142};
    matcha.tileColors[2048] = {122, 163, 98};
    themes["Matcha"] = matcha;

    Theme starryNight;
    starryNight.name = "StarryNight";
    starryNight.backgroundColor = {15, 20, 40};
    starryNight.gridColor = {25, 35, 60};
    starryNight.emptyTileColor = {35, 45, 75};
    starryNight.defaultTextColor = {245, 245, 250};
    starryNight.tileColors[2] = {75, 110, 155};
    starryNight.tileColors[4] = {95, 160, 175};
    starryNight.tileColors[8] = {60, 110, 200};
    starryNight.tileColors[16] = {35, 75, 175};
    starryNight.tileColors[32] = {45, 95, 145};
    starryNight.tileColors[64] = {140, 100, 70};
    starryNight.tileColors[128] = {190, 130, 80};
    starryNight.tileColors[256] = {200, 140, 60};
    starryNight.tileColors[512] = {215, 160, 50};
    starryNight.tileColors[1024] = {225, 175, 80};
    starryNight.tileColors[2048] = {235, 190, 150};
    themes["StarryNight"] = starryNight;
}

bool ThemeManager::setTheme(const std::string& themeName) {
    if (themes.find(themeName) != themes.end()) {
        currentThemeName = themeName;
        return true;
    }
    // Unknown or legacy saved theme names (including the removed Neon theme)
    // fall back to Classic instead of leaving a stale selection active.
    currentThemeName = "Classic";
    return false;
}

Theme ThemeManager::getCurrentTheme() const {
    return themes.at(currentThemeName);
}

std::vector<std::string> ThemeManager::getAvailableThemes() const {
    std::vector<std::string> themeNames;
    for (const auto& pair : themes) {
        themeNames.push_back(pair.first);
    }
    return themeNames;
}
