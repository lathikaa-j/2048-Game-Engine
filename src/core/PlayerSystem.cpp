#include "../../include/core/PlayerSystem.h"
#include <algorithm>
#include <cctype>

std::vector<std::string> PlayerSystem::getPlayers() const {
    std::vector<std::string> result;
    for (const auto& item : profiles) result.push_back(item.first);
    std::sort(result.begin(), result.end());
    return result;
}

bool PlayerSystem::createProfile(const std::string& username) {
    if (username.empty() || username.size() > 20 || username == "Guest"
        || username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos) return false;
    // Windows save paths ignore case, so profiles must not share a save slot.
    const auto folded = [](std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
    };
    if (folded(username) == "guest") return false;
    for (const auto& profile : profiles)
        if (folded(profile.first) == folded(username)) return false;
    if (profiles.find(username) != profiles.end()) {
        return false;
    }
    profiles[username] = {username};
    return true;
}

bool PlayerSystem::selectPlayer(const std::string& username) {
    if (profiles.find(username) != profiles.end()) {
        activePlayer = username;
        return true;
    }
    return false;
}

std::string PlayerSystem::getActivePlayer() const {
    return activePlayer;
}
