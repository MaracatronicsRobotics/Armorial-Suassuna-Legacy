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

class PlayerBoard
{
public:
    PlayerBoard();

    // Getters and Setters
    Robot getKeeper();
    void setKeeper(const Robot &keeper);

    QList<Robot> getBarriersList();
    void setBarriersList(const QList<Robot> &barriersList);

    QList<Robot> getMidfieldersList();
    void setMidfieldersList(const QList<Robot> &midfieldersList);

    QList<Robot> getForwardersList();
    void setForwardersList(const QList<Robot> &forwardersList);

    QList<Robot> getTeamList();
    void setTeamList(const QList<Robot> &teamList);

    // Adds
    void addBarrier(const Robot newBarrier);
    void addMidfielder(const Robot newMidfielder);
    void addForwarder(const Robot newForwarder);

    // clears
    void clearBarriersList();
    void clearMidfieldersList();
    void clearForwardersList();
    void clearTeamList();

    void clearAllLists();

private:
    Robot _keeper;
    QList<Robot> _barriersList;
    QList<Robot> _midfieldersList;
    QList<Robot> _forwardersList;
    QList<Robot> _teamList;


    // Locks
    QReadWriteLock _keeperLock;
    QReadWriteLock _barriersLock;
    QReadWriteLock _midfieldersLock;
    QReadWriteLock _forwardersLock;
    QReadWriteLock _teamLock;
};

#endif // PLAYERBOARD_H
