/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "team.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

#include <src/entities/player/player.h>

Team::Team(const Common::Enums::Color& teamColor) {
    _teamColor = teamColor;
}

void Team::addPlayer(Player *player) {
    _players.push_back(player);
}

Common::Enums::Color Team::teamColor() const {
    return _teamColor;
}

std::optional<Suassuna::Robot> Team::getPlayer(const quint8& robotId) const {
    for (const auto& p : _players) {
        if(p->robotId() == robotId) {
            Suassuna::Robot robot(*p);
            return robot;
        }
    }

    spdlog::warn("Invalid requested robot id: {}", robotId);
    return std::nullopt;
}

QList<Suassuna::Robot*> Team::getPlayers() const {
    QList<Suassuna::Robot*> robots;

    for (const auto& p : _players) {
        Suassuna::Robot* robot = new Suassuna::Robot(*p);
        robots.push_back(robot);
    }

    return robots;
}

void Team::updatePlayer(const Armorial::Robot &playerData) {
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
        if(p->robotId() == robotIdentifier.robotid()) {
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
