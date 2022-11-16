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

#include <Armorial/Common/Enums/Enums.h>

#include <include/proto/messages.pb.h>

#include <src/entities/player/player.h>

/*!
 * \brief The Team class provides a interface which describes a team with a given Color.
 */
class SSLTeam
{
public:
    /*!
     * \brief Team
     */
    SSLTeam(const Common::Enums::Color& teamColor);

    /*!
     * \brief Add a given Player pointer to this Team class.
     * \param player The given player.
     */
    void addPlayer(Player *player);

    /*!
     * \return The registered Color for this Team instance.
     */
    Common::Enums::Color teamColor() const;

    QList<quint8> getIDs();
    Player* getPlayer(quint8 id);

protected:
    friend class WorldMap;

    /*!
     * \brief Update a player in the list that matches the RobotIdentifier from the given player.
     * \param player The given player to update.
     */
    void updatePlayer(const Armorial::Robot& playerData);

private:
    QList<Player*> _players;
    QList<quint8> _ids;
    Common::Enums::Color _teamColor;
};

#endif // TEAM_H
