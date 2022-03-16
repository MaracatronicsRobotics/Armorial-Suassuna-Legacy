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

#include "fieldside.h"

FieldSide::FieldSide(Sides::SIDE side) {
    _side = side;
}

FieldSide FieldSide::oppositeSide() const {
    return FieldSide(_side == Sides::RIGHT ? Sides::LEFT : Sides::RIGHT);
}

bool FieldSide::isRight() const {
    return (_side == Sides::RIGHT);
}

bool FieldSide::isLeft() const {
    return (_side == Sides::LEFT);
}

bool FieldSide::isUndefined() const {
    return (_side == Sides::UNDEFINED);
}
