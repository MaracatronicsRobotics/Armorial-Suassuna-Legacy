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

#ifndef GOTO_H
#define GOTO_H

#include <src/entities/player/skill/skill.h>

class Skill_GoTo : public Skill
{
public:
    Skill_GoTo();
    QString name();

    // Target management
    void setTargetPosition(Position &targetPosition);
    void setSpeedFactor(float speedFactor);

private:
    // Skill inherited methods
    void configure();
    void run();

    // Internal
    Position _targetPosition;
    float _speedFactor;
    bool _avoidTeammates;
    bool _avoidOpponents;
    bool _avoidBall;
    bool _avoidOurGoalArea;
    bool _avoidTheirGoalArea;
};

#endif // GOTO_H
