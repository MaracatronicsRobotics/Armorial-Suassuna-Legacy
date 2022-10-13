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

#include <math.h>

#include <spdlog/spdlog.h>

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
}

void Behavior_Default::run() {
    Geometry::Vector2D center = getWorldMap()->getField().centerCircle().center();

    spdlog::info("distance = {}", player()->getPosition().dist(center));
    if(player()->getPosition().dist(center) >= 0.2f) {
        spdlog::info("teste1");
        _skill_goTo->setTargetPosition(center);
        runSkill(SKILL_GOTO);
    }
    else {
        spdlog::info("teste2");
        _skill_rotateTo->setTargetAngle(M_PI/2.0);
        runSkill(SKILL_ROTATETO);
    }
}
