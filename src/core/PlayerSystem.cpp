#include "../../include/core/PlayerSystem.h"

bool PlayerSystem::createProfile(const std::string& username) {
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