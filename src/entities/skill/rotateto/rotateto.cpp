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

#include "rotateto.h"

RotateTo::RotateTo() {
    _targetPosition = Utils::getPositionObject(0.0, 0.0);
    _referencePosition = Utils::getPositionObject(0.0, 0.0);
}

QString RotateTo::name() {
    return "Skill_RotateTo";
}

void RotateTo::setTargetPosition(Position &targetPosition) {
    _targetPosition = targetPosition;
}

void RotateTo::setReferencePosition(Position referencePosition) {
    _referencePosition = referencePosition;
}

void RotateTo::configure() {

}

void RotateTo::run() {
    if(!_targetPosition.isinvalid()) {
        player()->rotateTo(_targetPosition, _referencePosition);
    }
}
