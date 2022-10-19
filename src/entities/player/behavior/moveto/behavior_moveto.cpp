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

#include "behavior_moveto.h"
#include <spdlog/spdlog.h>

Behavior_MoveTo::Behavior_MoveTo() {
    _isRotationEnabled = false;
    _keepDistance = false;
    _spin = false;
    _spinClock = true;
    _spinSpeed = 40;
    _forceMotion = false;
    _antiStuck = true;
    _leftWheelPower = 0.0f;
    _rightWheelPower = 0.0f;
}

QString Behavior_MoveTo::name() {
    return "Behavior_MoveTo";
}

void Behavior_MoveTo::configure() {
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();
    _skill_spin = new Skill_Spin();
    _skill_move = new Skill_Move();

    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
    addSkill(SKILL_SPIN, _skill_spin);
    addSkill(SKILL_MOVE, _skill_move);
}

void Behavior_MoveTo::run() {
    if (_keepDistance) {
        // Maybe keep a distance to position
    }

//    spdlog::info("MOVETO Desired Position: ({}, {})", _desiredPosition.x(), _desiredPosition.y());

    if (_isRotationEnabled) {
        _skill_rotateTo->setTargetPosition(_desiredPosition);
        runSkill(SKILL_ROTATETO);
    } else if(_spin) {
        _skill_spin->setClockWise(_spinClock);
        _skill_spin->setSpinSpeed(_spinSpeed);
        runSkill(SKILL_SPIN);
    } else if(_forceMotion) {
        _skill_move->setLeftWheelPower(_leftWheelPower);
        _skill_move->setRightWheelPower(_rightWheelPower);
        runSkill(SKILL_MOVE);
    }
    else {
        _skill_goTo->setTargetPosition(_desiredPosition);
        _skill_goTo->enableWallAntiStuck(_antiStuck);
        runSkill(SKILL_GOTO);
    }
}
