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

#include "goto.h"

GoTo::GoTo() {
    _targetPosition = Utils::getPositionObject(0.0, 0.0);
}

QString GoTo::name() {
    return "Skill_GoTo";
}

void GoTo::setTargetPosition(Position &targetPosition) {
    _targetPosition = targetPosition;
}

void GoTo::configure() {

}

void GoTo::run() {
    if(!_targetPosition.isinvalid()) {
        player()->playerGoTo(_targetPosition);
    }
}
