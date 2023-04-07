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

#include "behavior_default.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Behavior_Default::Behavior_Default() {
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
    _skill_spin = nullptr;
}

void Behavior_Default::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();
    _skill_spin = new Skill_Spin();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
    addSkill(SKILL_SPIN, _skill_spin);

    // Defines initial target position
    _targetPosition = Geometry::Vector2D(0.5f, 0.5f);
}

void Behavior_Default::run() {
    // Test logic for the 3 main skills ocurring simultaneously
    if(player()->robotId() == 0) {
        // Move the robot to other positions and see if it rotate to the
        // target position using the smarter rotation diretion route
        _skill_rotateTo->setTargetAngle((_targetPosition - player()->getPosition()).angle());
        runSkill(SKILL_ROTATETO);
    } else if (player()->robotId() == 1) {
        // Move the robot to other positions and see if it moves
        // to the target position using the best face no move
        _skill_goTo->setTargetPosition(_targetPosition);
        _skill_goTo->setGUI(gui());
        runSkill(SKILL_GOTO);
    } else {
        // Move the robot to positions where you can see both spin directions
        if (_targetPosition.y() > player()->getPosition().y()) {
            _skill_spin->setClockwiseDirection(false);
        } else {
            _skill_spin->setClockwiseDirection(true);
        }
        runSkill(SKILL_SPIN);
    }
}
