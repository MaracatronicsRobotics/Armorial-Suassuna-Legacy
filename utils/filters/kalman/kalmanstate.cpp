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

#include "kalmanstate.h"

KalmanState::KalmanState() {

    _state.setSize(3, 1);

}

float KalmanState::getPosition() const {

    return _state.get(0, 0);

}

float KalmanState::getVelocity() const {

    return _state.get(1, 0);

}

float KalmanState::getAcceleration() const {

    return _state.get(2, 0);

}

Matrix& KalmanState::matrix() {

    return _state;

}

void KalmanState::setPosition(float value) {

    _state.set(0, 0, value);

}

void KalmanState::setVelocity(float value) {

    _state.set(1, 0, value);

}

void KalmanState::setAcceleration(float value) {

    _state.set(2, 0, value);

}
