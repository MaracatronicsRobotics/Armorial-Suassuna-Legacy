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

#ifndef WORLDMAPUPDATER_HH
#define WORLDMAPUPDATER_HH

#include <entity/player/baseplayer.h>
#include <utils/fields/fields.hh>
#include <GEARSystem/gearsystem.hh>

#include <iostream>
#include <fstream>

class WorldMapUpdater {
public:
    WorldMapUpdater(Controller *ctr, Fields::Field  *defaultField);
    ~WorldMapUpdater();
    void setDefaultFieldGeometry(WorldMap *wm);
    void update(WorldMap *wm);
private:
    // Internal updaters
    void updateFieldGeometry(WorldMap *wm);
    void updateBall(WorldMap *wm);
    void updateTeam(WorldMap *wm, quint8 teamId);
    void updateBallPossession(WorldMap *wm);

    // Controller access
    Controller *_ctr;

    // Default field
    Fields::Field *_defaultField;

    // Internal
    Position _lastBallPosition;
};

#endif // WORLDMAPUPDATER_HH
