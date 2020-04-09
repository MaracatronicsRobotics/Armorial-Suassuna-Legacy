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

#ifndef FIELDSIDE_HH
#define FIELDSIDE_HH

#include <utils/basics/side.hh>

class FieldSide {
private:
    Sides::SIDE _side;
    void set(Sides::SIDE side);
public:
    FieldSide(Sides::SIDE side = Sides::UNDEFINED);

    // Opposite side
    FieldSide oppositeSide() const;

    // Bool getters
    bool isRight() const;
    bool isLeft() const;
    bool isCenter() const;
    bool isUndefined() const;

    // Operators
    FieldSide& operator=(const FieldSide &fside);
    FieldSide& operator=(const Sides::SIDE &side);
};

#endif // FIELDSIDE_HH
