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

#include "behavior_chaser.h"
#include <src/constants/constants.h>

Behavior_Chaser::Behavior_Chaser() {
    _CHAAAAAAAAAAAAAAAAAAARGE = false;
}

void Behavior_Chaser::configure() {
    _skill_goTo = new Skill_GoTo();
    _skill_charge = new Skill_Charge();

    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_CHARGE, _skill_charge);
}

void Behavior_Chaser::run() {
    if (hammerTime()) {
        Geometry::Angle orientationToTarget = Geometry::Angle((_chasePos - player()->getPosition()).angle());
        Geometry::Angle orientationPlayer = player()->getOrientation();
        Geometry::Angle orientationPlayerBack = Geometry::Angle(player()->getOrientation().value() + M_PI);
        float frontAngle = orientationPlayer.shortestAngleDiff(orientationToTarget);
        float backAngle = orientationPlayerBack.shortestAngleDiff(orientationToTarget);

        _skill_charge->setUseSwappedOrientation(backAngle < frontAngle);
        runSkill(SKILL_CHARGE);
    } else {
        _skill_goTo->setTargetPosition(getChasePosition());
        runSkill(SKILL_GOTO);
    }
}

Geometry::Vector2D Behavior_Chaser::getChasePosition() {
    Geometry::Vector2D chasePos = _chasePos;
    bool condition;
    if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_RIGHT) {
        condition = player()->getPosition().x() <= _chasePos.x();
    } else {
        condition = player()->getPosition().x() >= _chasePos.x();
    }

    if (condition) {
        Geometry::Vector2D unitVector = getWorldMap()->getBall().getPosition() - getWorldMap()->getField().theirGoalCenter();
        chasePos = unitVector.normalize() * 1.1f * unitVector.length();

        if (!getWorldMap()->getField().field().contains(chasePos)) {
            chasePos = getWorldMap()->getBall().getPosition();
        }
    }

//    chargePos = getWorldMap()->getBall().getPosition();
    return chasePos;
}
