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

#include "skill_spin.h"

Skill_Spin::Skill_Spin() {
}

void Skill_Spin::configure() {
    _isClockwise = true;
    _spinSpeed = 50.0f;
}

void Skill_Spin::run() {
    if (_isClockwise) {
        setWheelsSpeed(_spinSpeed, -_spinSpeed);
    } else {
        setWheelsSpeed(-_spinSpeed, _spinSpeed);
    }
}

void Skill_Spin::setClockwiseDirection(const bool &isClockwise) {
    _isClockwise = isClockwise;
}

void Skill_Spin::setSpinSpeed(const float &spinSpeed) {
    _spinSpeed = spinSpeed;
}
