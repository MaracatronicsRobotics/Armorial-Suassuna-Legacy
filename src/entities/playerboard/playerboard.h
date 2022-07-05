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

#ifndef PLAYERBOARD_H
#define PLAYERBOARD_H

#include <QReadWriteLock>
#include <QList>

#include <src/utils/utils.h>
#include <src/entities/player/player.h>
#include <src/magic_enum.hpp>

class PlayerBoard
{
public:
    PlayerBoard();

    enum roles {
        ROLE_GOALKEEPER,
        ROLE_BARRIER,
        ROLE_MIDFIELDER,
        ROLE_ATTACKER
    };

    // gets

    QList<quint8> getPlayersIds (const roles role);
    QHash<quint8, Position> getPlayersPositions(const roles role);

    // sets
    void setPlayersList (QList<Player*> playerList);

    quint8 getClosestRolePlayerTo(const Position &target, const roles role);
    quint8 getClosestTeamPlayerTo(const Position &target);

private:
    QList<Player*> _playersList;
};

#endif // PLAYERBOARD_H
