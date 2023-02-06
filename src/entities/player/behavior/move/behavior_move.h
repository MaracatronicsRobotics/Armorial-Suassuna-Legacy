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

#ifndef BEHAVIOR_MOVE_H
#define BEHAVIOR_MOVE_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/goto/skill_goto.h>
#include <src/entities/player/skill/rotateto/skill_rotateto.h>
#include <src/entities/player/skill/spin/skill_spin.h>

class Behavior_Move : public Behavior
{
public:
    Behavior_Move();

    enum MOVEMENT {
        MOVE,
        ROTATE,
        SPIN
    };

    void setMovementType(uint movementType) { _movementType = movementType; }
    void setTargetPosition(Geometry::Vector2D targetPosition) { _targetPosition = targetPosition; }
    void setTargetAngle(float targetAngle) { _targetAngle = targetAngle; }
    void setSpinSpeed(float spinSpeed) { _spinSpeed = spinSpeed; }
    void setClockwiseSpin(bool isClockwise) { _isClockwise = isClockwise; }

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills pointers
    Skill_GoTo *_skill_goTo;
    Skill_RotateTo *_skill_rotateTo;
    Skill_Spin *_skill_spin;

    // Skills enum
    enum {
        SKILL_GOTO,
        SKILL_ROTATETO,
        SKILL_SPIN
    };

    uint _movementType;
    Geometry::Vector2D _targetPosition;
    float _targetAngle;
    float _spinSpeed;
    bool _isClockwise;
};

#endif // BEHAVIOR_MOVE_H
