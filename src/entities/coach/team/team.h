
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

#ifndef TEAM_H
#define TEAM_H

#include <src/entities/basesuassuna.h>
#include <src/common/types/robot/robot.h>

class Team
{
public:
    /*!
     * \brief Team constructor.
     * \param teamColor The given team color for this Team instance.
     */
    Team(const Common::Enums::Color& teamColor);

    /*!
     * \brief Add a given Player pointer to this Team class.
     * \param player The given player pointer.
     */
    void addPlayer(Player *player);

    /*!
     * \return The registered Color for this Team instance.
     */
    Common::Enums::Color teamColor() const;

    /*!
     * \param robotId The given id.
     * \return A robot with a given id on this Team instance.
     */
    std::optional<Suassuna::Robot> getPlayer(const quint8& robotId) const;

    /*!
     * \return The robots registered for this Team instance.
     */
    QList<Suassuna::Robot*> getPlayers() const;

protected:
    friend class WorldMap;

    /*!
     * \brief Update a player in the list that matches the RobotIdentifier from the given player.
     * \param player The given player to update.
     */
    void updatePlayer(const Armorial::Robot& playerData);

private:
    // Internal objects
    QList<Player*> _players;
    Common::Enums::Color _teamColor;
};

#endif // TEAM_H
