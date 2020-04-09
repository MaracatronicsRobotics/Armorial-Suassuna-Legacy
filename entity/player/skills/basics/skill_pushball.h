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

#ifndef SKILL_PUSHBALL_H
#define SKILL_PUSHBALL_H

#include <entity/player/skills/skill.h>

class Skill_PushBall : public Skill {
private:
    // Parameters
    Position _destination;
    float _maxPushDistance;

    // State machine
    enum {
        STATE_POS,
        STATE_PUSH
    };
    int _state;

    // Internal
    Position _currPos;
    Position _lastPos;
    float _distPushed;
    void run();
    bool isBehindBall(Position posObjective);
    bool isBallInFront();
public:
    Skill_PushBall();
    QString name();
    void setDestination(const Position &destination) { _destination = destination; }
    void setMaxPushDistance(float maxPushDistance) { _maxPushDistance = maxPushDistance; }
};

#endif // SKILL_PUSHBALL_H
