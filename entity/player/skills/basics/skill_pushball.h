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
#include <utils/freeangles/freeangles.h>

class Skill_PushBall : public Skill {
private:
    // Parameters
    Position _destination;
    Position _aim;

    // Internal
    bool _shootWhenAligned;
    Position _currPos;
    Position _lastPos;
    float _maxPushDistance;
    float _pushedDistance;
    float _kickPower;
    bool _isParabolic;
    bool _isPenalty;

    // State machine
    enum {
        STATE_POS,
        STATE_TRY,
        STATE_PUSH
    };
    int _state;

    void run();
    bool isBallInFront();
    bool isBehindBall(Position posObjective);

public:
    Skill_PushBall();
    QString name();
    // kick
    void shootWhenAligned(bool shootWhenAligned) { _shootWhenAligned = shootWhenAligned; }
    void setKickPower(float kickPower) { _kickPower = kickPower; }
    void setIsParabolic(bool isParabolic) { _isParabolic = isParabolic; }
    // Push
    void setMaxPushDistance(float dist) { _maxPushDistance = dist; }
    void setDestination(const Position &destination) { _destination = destination; }
    void setAim(const Position &aim) { _aim = aim; }
    float getMaxPushDistance() { return _maxPushDistance; }
    float getPushedDistance() { return _pushedDistance; }
    // For penalty shoots
    void setIsPenalty(bool isPenalty) { _isPenalty = isPenalty; }
};

#endif // SKILL_PUSHBALL_H
