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

#include "behavior_barrier.h"

#include <math.h>

#include <spdlog/spdlog.h>

Behavior_Barrier::Behavior_Barrier() {
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
    _skill_spin = nullptr;

    setRadius(0.29f);
}

QString Behavior_Barrier::name() {
    return "Behavior_Barrier";
}

void Behavior_Barrier::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_spin = new Skill_Spin();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_SPIN,_skill_spin);
}

void Behavior_Barrier::run() {
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

bool Behavior_Barrier::spinOrientarion() {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    Geometry::Vector2D playerPos = player()->getPosition();

    if(!getWorldMap()->getField().playingLeftSide()) {
        if(ballPos.y() > playerPos.y()) {
            return false;
        }else {
            return true;
        }
    }else {
        if(ballPos.y() > playerPos.y()) {
            return true;
        }else {
            return false;
        }
    }
}
