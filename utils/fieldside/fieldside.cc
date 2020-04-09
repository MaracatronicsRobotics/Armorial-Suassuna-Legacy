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

#include "fieldside.hh"

using namespace Sides;

FieldSide::FieldSide(SIDE side) {
    set(side);
}

FieldSide FieldSide::oppositeSide() const {
    return FieldSide(_side==RIGHT? LEFT : RIGHT);
}

void FieldSide::set(SIDE side) {
    _side = side;
}

bool FieldSide::isRight() const {
    return (_side==RIGHT);
}

bool FieldSide::isLeft() const {
    return (_side==LEFT);
}

bool FieldSide::isCenter() const {
    return(_side==CENTER);
}

bool FieldSide::isUndefined() const {
    return (_side==UNDEFINED);
}

FieldSide& FieldSide::operator=(const FieldSide &fside) {
    set(fside._side);
    return *this;
}

FieldSide& FieldSide::operator=(const SIDE &side) {
    set(side);
    return *this;
}
