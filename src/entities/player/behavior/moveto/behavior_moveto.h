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

#ifndef BEHAVIOR_MOVETO_H
#define BEHAVIOR_MOVETO_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/skills.h>

class Behavior_MoveTo : public Behavior
{
public:
    Behavior_MoveTo();
    QString name();

    // Setters
    void setPosition(Geometry::Vector2D desiredPosition) { _desiredPosition = desiredPosition; }
    void keepDistance(bool keepDistance) { _keepDistance = keepDistance; }
    void enableRotation(bool isRotationEnabled) { _isRotationEnabled = isRotationEnabled; }

private:
    void configure();
    void run();

    enum {
        SKILL_GOTO,
        SKILL_ROTATETO
    };

    Skill_GoTo *_skill_goTo;
    Skill_RotateTo *_skill_rotateTo;

    bool _isRotationEnabled;

    Geometry::Vector2D _desiredPosition;

    bool _keepDistance;
};

#endif // BEHAVIOR_MOVETO_H
