#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include <string>
#include <unordered_map>

struct PlayerProfile {
    std::string username;
};

class PlayerSystem {
private:
    std::unordered_map<std::string, PlayerProfile> profiles;
    std::string activePlayer;

public:
    bool createProfile(const std::string& username);
    bool selectPlayer(const std::string& username);
    std::string getActivePlayer() const;
};

#endif