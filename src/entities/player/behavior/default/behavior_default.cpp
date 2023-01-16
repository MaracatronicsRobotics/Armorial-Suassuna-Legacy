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
}

void Behavior_Default::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);

    // Defines player pos based on id
    Geometry::LineSegment segment = getWorldMap()->getField().ourPenaltyArea().rightLine();
    if(player()->robotId() == 0) {
        _targetPosition = segment.start();
    }
    else if(player()->robotId() == 1) {
        _targetPosition = Geometry::Vector2D(segment.start().x(), (segment.start().y() + segment.end().y())/2.0);
    }
    else {
        _targetPosition = segment.end();
    }
}

void Behavior_Default::run() {
    if(player()->getPosition().dist(_targetPosition) < 0.05f) {
        _skill_rotateTo->setTargetAngle(M_PI/2.0f);
        runSkill(SKILL_ROTATETO);
    }
    else {
        _skill_goTo->setTargetPosition(_targetPosition);
        runSkill(SKILL_GOTO);
    }
}
