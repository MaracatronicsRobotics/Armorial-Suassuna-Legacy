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

#ifndef SKILL_KICK_H
#define SKILL_KICK_H

#include <src/entities/skill/skill.h>
#include <src/utils/utils.h>
#include <src/entities/worldmap/worldmap.h>

class Kick : public Skill {
private:
    void run();
    Position _aimPosition;

    enum{
        STATE_POS,
        STATE_KICK
    };

    int _state;
    float _power;
    bool _isChip;
    WorldMap *_worldMap;
    bool isBehindBall(Position posObjective);
    bool isBallInFront();
    bool isInFrontOfObjective();

public:
    Kick(WorldMap *worldMap);
    QString name();

    void setAim (Position pos) { _aimPosition = pos; }
    void setPower(float power) { _power = power; }
    void setIsChip(bool isChip) { _isChip = isChip; }
    void setState(int state) { _state = state; }
    WorldMap* getWorldMap();
};

#endif // SKILL_KICK_H
