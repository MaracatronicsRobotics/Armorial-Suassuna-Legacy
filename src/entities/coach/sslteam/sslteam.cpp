#include "sslteam.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

SSLTeam::SSLTeam(const Common::Enums::Color& teamColor) {
    _teamColor = teamColor;
}

void SSLTeam::addPlayer(Player *player) {
    _players.push_back(player);
}

Common::Enums::Color SSLTeam::teamColor() const {
    return _teamColor;
}

void SSLTeam::updatePlayer(const Armorial::Robot &playerData) {
    // Take RobotIdentifier
    Armorial::RobotIdentifier robotIdentifier = playerData.robotidentifier();

    // Check if matches team color
    Common::Enums::Color robotTeamColor = (robotIdentifier.robotcolor().isblue() ? Common::Enums::Color::BLUE : Common::Enums::Color::YELLOW);
    if(robotTeamColor != teamColor()) {
        spdlog::warn("[{}] Tried to add a player from the opposite color.", fmt::format(fmt::fg(teamColor() == Common::Enums::Color::BLUE ? fmt::terminal_color::blue : fmt::terminal_color::yellow) | fmt::emphasis::bold, (teamColor() == Common::Enums::Color::BLUE ? "Team BLUE": "Team YELLOW")));
        return ;
    }

    // Search for a registered player that matches the id
    for (auto p : _players) {
        if(p->playerId() == robotIdentifier.robotid()) {
            // Create Object from the given playerData
            p->updatePlayer(Common::Types::Object({playerData.robotposition().x(), playerData.robotposition().y()},
                                                  {playerData.robotvelocity().vx(), playerData.robotvelocity().vy()},
                                                  {playerData.robotacceleration().ax(), playerData.robotacceleration().ay()},
                                                  playerData.robotorientation().value(),
                                                  playerData.robotangularspeed().vw()));
            break;
        }
    }
}
