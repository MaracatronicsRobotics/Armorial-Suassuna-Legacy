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

#ifndef FIELD_HH
#define FIELD_HH

#include <QString>

namespace Fields {

class Field {
public:
    virtual ~Field();
    virtual QString name() const = 0;

    // Field dimensions
    virtual float length() const = 0;
    virtual float width() const = 0;

    // Goal dimensions
    virtual float goalWidth() const = 0;
    virtual float goalDepth() const = 0;

    // Center dimensions
    virtual float centerRadius() const = 0;

    // Defense area dimensions
    virtual float defenseRadius() const = 0;
    virtual float defenseStretch() const = 0;
    float defenseLength() const;
    float defenseWidth() const;

    // Min/max X and Y
    float minX() const;
    float maxX() const;
    float minY() const;
    float maxY() const;
};

}

#endif // FIELD_HH
