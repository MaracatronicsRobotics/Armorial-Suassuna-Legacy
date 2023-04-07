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

#include "behavior_move.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Behavior_Move::Behavior_Move() {
    _targetPosition = Geometry::Vector2D(0.0f, 0.0f);
    _targetAngle = 0.0f;
    _spinSpeed = 50.0f;
    _isClockwise = true;
}

void Behavior_Move::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();
    _skill_spin = new Skill_Spin();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
    addSkill(SKILL_SPIN, _skill_spin);

    _movementType = MOVEMENT::MOVE;
}

void Behavior_Move::run() {
    switch(_movementType) {
        case MOVEMENT::MOVE:
            _skill_goTo->setTargetPosition(_targetPosition);
            _skill_goTo->setGUI(gui());
            runSkill(SKILL_GOTO);
            break;
        case MOVEMENT::ROTATE:
            _skill_rotateTo->setTargetAngle(_targetAngle);
            runSkill(SKILL_ROTATETO);
            break;
        case MOVEMENT::SPIN:
            _skill_spin->setSpinSpeed(_spinSpeed);
            _skill_spin->setClockwiseDirection(_isClockwise);
            runSkill(SKILL_SPIN);
            break;
        default:
            _skill_goTo->setTargetPosition(Geometry::Vector2D(0.0f, 0.0f));
            runSkill(SKILL_GOTO);
    }
}
