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


#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <QReadWriteLock>

#include <src/entities/entity.h>
#include <src/services/coach/coachservice.h>
#include <src/entities/worldmap/locations/locations.h>
#include <src/entities/player/player.h>

class WorldMap : public Entity
{
public:
    WorldMap(Constants *constants);

    //
    QString name();

    // Data getters
    Robot getRobot(RobotIdentifier identifier);
    QList<Robot> getRobots(Color color);
    QList<int> getRobotsIDs(Color color);
    Player* getPlayerPointer(int playerID);
    Player getPlayer(Color color, int playerID);
    Field getField();
    Ball getBall();

    // Locations getter
    Locations* getLocations();

private:
    // Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Internal data
    QMap<bool, QList<Robot>> _robots;
    Ball _ball;
    Field _field;
    QReadWriteLock _mutex;

    // Locations
    Locations *_locations;

    // Coach Service
    CoachService *_coachService;
    CoachService* getService();

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Players
    QMap<int, Player*> _playerPointers;
    QMap<Color, QMap<int, Player>*> _playerObjects;
};

#endif // WORLDMAP_H
