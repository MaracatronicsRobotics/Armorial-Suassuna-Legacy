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

#ifndef SKILL_PUSHBALL2_H
#define SKILL_PUSHBALL2_H

#include <entity/player/skills/skill.h>

class Skill_PushBall2 : public Skill {
private:
    // Parameters
    Position _destination;
    Position _aim;

    // Internal
    Position _currPos;
    Position _lastPos;
    double _maxPushDistance;
    double _pushedDistance;

    // State machine
    enum {
        STATE_POS,
        STATE_PUSH,
        STATE_DONE
    };
    int _state;

    void run();
    bool isBehindBall(Position posObjective);
    bool isBallInFront();
    bool isInFrontOfObjective();

public:
    Skill_PushBall2();
    QString name();
    void setDestination(const Position &destination) { _destination = destination; }
    void setAim(const Position &aim) { _aim = aim; }
    void setMaxPushDistance(double dist) { _maxPushDistance = dist; }
    double getMaxPushDistance() { return _maxPushDistance; }
    double getPushedDistance() { return _pushedDistance; }
};

#endif // SKILL_PUSHBALL2_H
